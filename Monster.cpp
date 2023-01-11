// Fill out your copyright notice in the Description page of Project Settings.
#include "Monster.h"

#include "GridActor.h"
#include "Pathfinding.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AMonster::HomePosKey(TEXT("HomePos"));
const FName AMonster::PatrolPosKey(TEXT("PatrolPos"));
const FName AMonster::TargetKey(TEXT("Target"));

AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsMoving = false;
	bCheckLocation = false;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprints/AI/BB_Monster.BB_Monster"));
	if (BBObject.Succeeded())
		BBAsset = BBObject.Object;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/AI/BT_Monster.BT_Monster"));
	if (BTObject.Succeeded())
		BTAsset = BTObject.Object;

	Path.Reserve(1000);
	Path.Reset();
}

void AMonster::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	SetGridActor();
	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		//Blackboard->SetValueAsObject(AMonster::TargetKey, nullptr);
		if (!RunBehaviorTree(BTAsset))
			UE_LOG(LogTemp, Warning, TEXT("AIController couldn't run behavior tree!"));
	}
}

void AMonster::OnUnPossess()
{
	Super::OnUnPossess();
	//GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bCheckLocation)
	{
		FVector tmp = Blackboard->GetValueAsVector(PatrolPosKey);
		FVector2D actorLoc = FVector2D(GetPawn()->GetActorLocation().X, GetPawn()->GetActorLocation().Y);
		FVector2D current = FVector2D(tmp.X, tmp.Y);
		bIsMoving = !actorLoc.Equals(current, 15.f);
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		
		if(!bIsMoving)
		{
			//UE_LOG(LogTemp, Error, TEXT("Arrive!!!!!!!!!"));
			bCheckLocation = false;
		}
	}
}

void AMonster::SetGridActor()
{
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(GetPawn());
	GetWorld()->LineTraceSingleByChannel(Hit, GetPawn()->GetActorLocation(),
		GetPawn()->GetActorLocation() - GetPawn()->GetActorUpVector() * 100.f, ECollisionChannel::ECC_WorldDynamic, CollisionParams);

	if(Hit.GetActor())
	{
		UE_LOG(LogTemp, Error, TEXT("Monster Get !! %s"), *Hit.GetActor()->GetName());
		AGridActor* actor = Cast<AGridActor>(Hit.GetActor());
		if(actor) gridActor = actor;
	}
}

bool AMonster::SetPath(const FVector& _dst)
{
	if(gridActor && GetPawn())
	{
		bool result = false;
		Path = gridActor->GetPathfindComp()->IsFindPath(GetPawn()->GetActorLocation(),
			_dst, result);

		if(Path.Num() > 0) Path.Pop();

		return result;
	}

	return false;
}
