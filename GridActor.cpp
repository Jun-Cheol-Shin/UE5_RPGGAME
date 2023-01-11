// Fill out your copyright notice in the Description page of Project Settings.


#include "GridActor.h"

#include "NetworkMessage.h"
#include "Pathfinding.h"
#include "PathRequestManager.h"
#include "RPGCharacter.h"
#include "Components/BillboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AGridActor::AGridActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	G_Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	G_Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	gridFloor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));

	if(G_Scene)
	{
		RootComponent = G_Scene;
		if(G_Billboard) G_Billboard->SetupAttachment(RootComponent);
		gridFloor->SetupAttachment(RootComponent);
	}
	
	PathfindingComponent = CreateDefaultSubobject<UPathfinding>(TEXT("PathfindingComponent"));
	RequestManagerComponent = CreateDefaultSubobject<UPathRequestManager>(TEXT("RequestManager"));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("StaticMesh'/Engine/MapTemplates/SM_Template_Map_Floor.SM_Template_Map_Floor'"));
	if (CubeAsset.Succeeded())
	{
		gridFloor->SetStaticMesh(CubeAsset.Object);
		gridFloor->SetCollisionProfileName(UCollisionProfile::CustomCollisionProfileName);
		gridFloor->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel1, ECollisionResponse::ECR_Block);
	}
}

// Called when the game starts or when spawned
void AGridActor::BeginPlay()
{
	Super::BeginPlay();
	CreateGrid();

	/*Character = Cast<ARPGCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if(Character)
	{
		seeker = Character->GetActorLocation();
		//Character->SetPathRequestManager(this->RequestManagerComponent);
		//Character->SetGridActor(this);
	}*/

	SetActorRotation(FRotator(0,0,0));
	//gridFloor->SetRelativeScale3D(FVector(gridWorldSize.X / 500.f, gridWorldSize.Y / 500.f, 1.f));
	//if(PathfindingComponent) PathfindingComponent->FindPath(seeker, FVector(450, 450, 0));
}

FVector AGridActor::gridBottomLeft() const
{
	FVector temp = gridWorldSize.X * RootComponent->GetForwardVector();
	FVector temp2 = gridWorldSize.Y * RootComponent->GetRightVector();

	return (gridLocation - temp) - temp2;
}

FVector2D AGridActor::gridTileNumber() const
{
	FVector2D temp = gridWorldSize / nodeSize;

	temp.X = round(temp.X);
	temp.Y = round(temp.Y);

	return temp;
}

bool AGridActor::mySphereTrace(const FVector& Location, const float& size, const ECollisionChannel& type, FHitResult& result) const
{
	TArray<AActor*> arr;
	bool retval;
	retval = UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
				Location, Location, size,
				UEngineTypes::ConvertToTraceType(type), 
				false, arr,
				EDrawDebugTrace::None, result, true);

	return retval;
}

TArray<FSTNode*> AGridActor::GetNeighbours(FSTNode* node)
{
	TArray<FSTNode*> neighbours;

	for(int i = -1; i <= 1; ++i)
	{
		for(int j = -1; j <= 1; ++j)
		{
			if(i == 0 && j == 0)
			{
				continue;
			}

			int checkX = node->gridX + j;
			int checkY = node->gridY + i;

			if(grid.IsValidIndex(checkX) && grid[checkX].array.IsValidIndex(checkY))
			{
				if(checkX >= 0 && checkX < grid.Num() && checkY >= 0 && checkY < grid[checkX].array.Num())
				{
					neighbours.Add(grid[checkX].array[checkY]);
				}
			}
		}
	}

	return neighbours;
}

void AGridActor::CreateGrid()
{
	bool first = false;
	TArray<AActor*> arr;
	gridSize = gridTileNumber();
	FHitResult result;
	FVector a, b, TilePosition;

	//UE_LOG(LogTemp, Log, TEXT("grid count : %f %f"),gridSize.X, gridSize.Y);
	
	for(int i = 0; i < UE4::SSE::RoundToInt32(gridSize.X); ++i)
	{
		F2DArray tmp;
		a = RootComponent->GetForwardVector() * (i * (nodeSize * 2) + nodeSize);
		for(int j = 0; j < UE4::SSE::RoundToInt32(gridSize.Y); ++j)
		{
			b = RootComponent->GetRightVector() * (j * (nodeSize * 2) + nodeSize);
			TilePosition = gridBottomLeft() + a + b + GetActorLocation();

			if(mySphereTrace(TilePosition, nodeSize - 5, ECC_GameTraceChannel1, result))	// 충돌되는 액터가 있는 경우...
			{
				if(!first)
				{
					first = true;
					firstNodeLocation = TilePosition;
				}
				bool flag = true;
				if(mySphereTrace(TilePosition, nodeSize - 5, ECC_GameTraceChannel2, result)) flag = false;
				FSTNode* node = new FSTNode(flag, TilePosition, grid.Num(), tmp.array.Num());
				tmp.array.Add(node);
			}

			else
			{
				if(!first)
				{
					first = true;
					firstNodeLocation = TilePosition;
				}
				FSTNode* node = new FSTNode(false, TilePosition, grid.Num(), tmp.array.Num());
				tmp.array.Add(node);
			}
		}

		if(tmp.array.Num() > 0) grid.Add(tmp);
	}
	

	/*for(auto iter : grid)
	{
		for(auto iter2 : iter.array)
		{
			UE_LOG(LogTemp, Log, TEXT("%f, %f, %f  %d, %d"), iter2->worldPosition.X, iter2->worldPosition.Y, iter2->worldPosition.Z,
				iter2->gridX, iter2->gridY);
		}
	}
	*/
}

void AGridActor::DrawGrid()
{
	if(!bDisplayGridGizmos) return;
	
	for(auto iter : grid)
	{
		for(auto iter2 : iter.array)
		{
			FColor color = (iter2->bIsWalkable) ? FColorList::White : FColorList::Red;

			/*if(Character && Character->GetPlayerPath().Contains(iter2->worldPosition))
			{
				color = FColorList::Cyan;
			}*/
			DrawDebugSolidPlane(GetWorld(), FPlane(0,0,1,iter2->worldPosition.Z),
						iter2->worldPosition, nodeSize - 5,
						color, false, 0.1f, 1);
			//UE_LOG(LogTemp, Log, TEXT("%f, %f"), iter2->worldPosition.X, iter2->worldPosition.Y);
		}
	}
}

FSTNode* AGridActor::NodeFromWorldPoint(const FVector& worldPos)
{
	// 현재 사각형의 지대에서만 적용 가능한 점화식
	// 원 모양, 사다리꼴 모양처럼 가로 세로가 줄마다 일정하지 않은 경우에는?
	FVector tmp = (worldPos - firstNodeLocation) / (nodeSize * 2);


	//UE_LOG(LogTemp, Error, TEXT("%f, %f"), firstNodeLocation.X, firstNodeLocation.Y);
	//UE_LOG(LogTemp, Error, TEXT("%f, %f"), worldPos.X, worldPos.Y);
	
	int x = UE4::SSE::RoundToInt32(tmp.X);
	int y = UE4::SSE::RoundToInt32(tmp.Y);
	
	//UE_LOG(LogTemp, Error, TEXT("%d %d"), x, y);

	if(grid.IsValidIndex(x) && grid[x].array.IsValidIndex(y)) return grid[x].array[y];
	return nullptr;
}

// Called every frame
void AGridActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//DrawGrid();
}

void AGridActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	FlushPersistentDebugLines(GetWorld());
	gridLocation = RootComponent->GetComponentToWorld().GetLocation();
	DrawDebugBox(GetWorld(), RootComponent->GetComponentToWorld().GetLocation(),
		FVector(gridWorldSize.X, gridWorldSize.Y, 5), FColorList::Black,
		true, -1.f, 0, 10);

	//gridFloor->SetRelativeScale3D(FVector(gridWorldSize.X / 500.f, gridWorldSize.Y / 500.f, 1.f));
}

