// Fill out your copyright notice in the Description page of Project Settings.


#include "Ladder.h"

#include "ClimberComponent.h"
#include "RPGCharacter.h"
#include "StaticMeshDescription.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
ALadder::ALadder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	climberTransform = CreateDefaultSubobject<USceneComponent>(TEXT("ClimberTransform"));
	ladder_Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ladder_top"));
	cylinder_L = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ladder_L"));
	cylinder_R = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ladder_R"));
	crossBar = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ism_crossbar"));
	collisionBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ladderStart"));
	
	RootComponent = DefaultSceneRoot;
	cylinder_L->SetupAttachment(DefaultSceneRoot);
	cylinder_R->SetupAttachment(DefaultSceneRoot);
	ladder_Top->SetupAttachment(DefaultSceneRoot);
	crossBar->SetupAttachment(DefaultSceneRoot);
	collisionBox->SetupAttachment(DefaultSceneRoot);
	climberTransform->SetupAttachment(DefaultSceneRoot);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (CylinderAsset.Succeeded())
	{
		cylinder_L->SetStaticMesh(CylinderAsset.Object);
		cylinder_L->SetRelativeLocation(FVector(0.0f, -30.0f, 0.0f));
		cylinder_L->SetWorldScale3D(FVector(0.1f,0.1f, 1.f));
		
		cylinder_R->SetStaticMesh(CylinderAsset.Object);
		cylinder_R->SetRelativeLocation(FVector(0.0f, 30.0f, 0.0f));
		cylinder_R->SetWorldScale3D(FVector(0.1f,0.1f, 1.f));
	}
	
	if (CubeAsset.Succeeded())
	{
		ladder_Top->SetStaticMesh(CubeAsset.Object);
		ladder_Top->SetRelativeLocation(FVector::Zero());
		ladder_Top->SetWorldScale3D(FVector(0.1f,0.1f,0.1f));
		ladder_Top->SetHiddenInGame(true);
		ladder_Top->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

		crossBar->SetStaticMesh(CubeAsset.Object);
		//m_cube->SetWorldScale3D(FVector(0.05f, 0.6f, 0.05f));
		crossBar->SetRelativeLocation(FVector(0,0,20.f));

		collisionBox->SetStaticMesh(CubeAsset.Object);
	}

	// 박스 이벤트 크기 조정
	collisionBox->SetRelativeScale3D(FVector(2.f,1.f,1.f));
	collisionBox->SetRelativeLocation(FVector(0.f,0.f,30.f));
	collisionBox->SetVisibility(false);
	collisionBox->SetHiddenInGame(true);
	collisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	collisionBox->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnOverlapBegin);
	collisionBox->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnOverlapEnd);
	climberTransform->SetRelativeLocation(FVector(45,0,0));
	climberTransform->SetRelativeRotation(FRotator(0,-90,0));


	cylinder_R->SetCollisionProfileName(UCollisionProfile::CustomCollisionProfileName);
	cylinder_L->SetCollisionProfileName(UCollisionProfile::CustomCollisionProfileName);
	crossBar->SetCollisionProfileName(UCollisionProfile::CustomCollisionProfileName);
	crossBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	crossBar->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel2, ECollisionResponse::ECR_Block);
	cylinder_R->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel2, ECollisionResponse::ECR_Block);
	cylinder_L->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel2, ECollisionResponse::ECR_Block);
}

void ALadder::InitLadder() const
{
	ladder_Top->SetRelativeLocation(FVector(0,0,height));
	AdjustSideBar(cylinder_L);
	AdjustSideBar(cylinder_R);
	//AdjustSideBar(collisionBox);

	collisionBox->SetRelativeLocation(FVector(collisionBox->GetRelativeLocation().X,
		collisionBox->GetRelativeLocation().Y,ladder_Top->GetRelativeLocation().Z / 2.f));
	collisionBox->SetRelativeScale3D(FVector(collisionBox->GetRelativeScale3D().X,
		collisionBox->GetRelativeScale3D().Y,ladder_Top->GetRelativeLocation().Z / 90.f));
	
	crossBar->ClearInstances();
	
	float tempHeight = 0.f;
	
	while(tempHeight < ladder_Top->GetRelativeLocation().Z)
	{
		FTransform transform;
		transform.SetScale3D(FVector(0.05f,0.6f,0.05f));
		transform.SetLocation(FVector(0,0,tempHeight));
		crossBar->AddInstance(transform);
		tempHeight += crossbarGap;
	}
}

// Called when the game starts or when spawned
void ALadder::BeginPlay()
{
	Super::BeginPlay();
	InitLadder();
}

// Called every frame
void ALadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ALadder::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	InitLadder();
}

void ALadder::AdjustSideBar(USceneComponent* comp) const
{
	FVector location = FVector(comp->GetRelativeLocation().X,
		comp->GetRelativeLocation().Y, ladder_Top->GetRelativeLocation().Z / 2.f);

	comp->SetRelativeScale3D(FVector(comp->GetRelativeScale3D().X,
	comp->GetRelativeScale3D().X, ladder_Top->GetRelativeLocation().Z / 100.f));
	comp->SetRelativeLocation(location);
}


void ALadder::OnOverlapBegin(UPrimitiveComponent* selfComp, AActor* otherActor, UPrimitiveComponent* otherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("In Ladder!"));
	
	if(ARPGCharacter* character = Cast<ARPGCharacter>(otherActor))
	{
		//climbComp = character->GetClimbComp();
		check(climbComp);
		climbComp->SetLadder(this);
	}
}

void ALadder::OnOverlapEnd(UPrimitiveComponent* selfComp, AActor* otherActor, UPrimitiveComponent* otherComp,
	int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Error, TEXT("Out Ladder!"));
	if(ARPGCharacter* character = Cast<ARPGCharacter>(otherActor))
	{
		//climbComp = character->GetClimbComp();
		check(climbComp);
		climbComp->SetLadder(nullptr);
	}
}

