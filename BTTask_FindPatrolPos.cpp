// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"

#include "Monster.h"
#include "GridActor.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPosAndMove");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if(!monsterAIController) monsterAIController = Cast<AMonster>(OwnerComp.GetAIOwner());
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	if (nullptr == monsterAIController || nullptr == ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AMonster::HomePosKey);
	FVector NextLocation;

	if(monsterAIController->GetGridActor())
	{
		TArray<FSTNode*> retval = monsterAIController->GetGridActor()->GetNeighbours(monsterAIController->GetGridActor()->NodeFromWorldPoint(ControlledPawn->GetActorLocation()));

		if(retval.Num() > 0)
		{
			int idx = 0;
			idx = FMath::RandRange(0, retval.Num() - 1);

			if(retval[idx]->bIsWalkable)
			{
				NextLocation = retval[idx]->worldPosition;
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMonster::PatrolPosKey, NextLocation);
				monsterAIController->SetCheckMoving(true);
				monsterAIController->MoveToLocation(NextLocation,-1,false,false);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	
	return EBTNodeResult::Failed;
}
