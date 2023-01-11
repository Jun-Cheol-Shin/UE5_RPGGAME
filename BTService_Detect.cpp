// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"

#include "Monster.h"
#include "MonsterActor.h"
#include "RPGCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	
	AMonsterActor* ControllingPawn = Cast<AMonsterActor>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	
	float DetectRadius = ControllingPawn->GetDetectRange();

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);
	
	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			ARPGCharacter* ABCharacter = Cast<ARPGCharacter>(OverlapResult.GetActor());
			//컨트롤러가 플레이어의 컨트롤러인지
			if (ABCharacter && ABCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonster::TargetKey, ABCharacter);
				//반경을 보여준다
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				
				//DrawDebugPoint(World, ABCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				//DrawDebugLine(World, ControllingPawn->GetActorLocation(), ABCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			} 
		}
	}
	else
	{
		//null로 설정해준다.
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonster::TargetKey, nullptr);
		// 처음 자리로 돌아가는 로직....
	}


	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
