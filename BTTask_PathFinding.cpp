// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PathFinding.h"

#include "GridActor.h"
#include "Monster.h"
#include "Pathfinding.h"
#include "RPGCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_PathFinding::UBTTask_PathFinding()
{
	NodeName = TEXT("PathFinding");
	targetIndex = 0;
}

EBTNodeResult::Type UBTTask_PathFinding::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	if(!monsterAIController) monsterAIController = Cast<AMonster>(OwnerComp.GetAIOwner());
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	ARPGCharacter* target = Cast<ARPGCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonster::TargetKey));

	if(target)
	{
		auto TargetNode = monsterAIController->GetGridActor()->NodeFromWorldPoint(target->GetActorLocation());
		auto TargetKeyNode = monsterAIController->GetGridActor()->NodeFromWorldPoint(
				OwnerComp.GetBlackboardComponent()->GetValueAsVector(AMonster::TargetKey));
		// 캐릭터와 몬스터 사이의 최단경로 구하기...

		if(monsterAIController->GetPath().Num() == 0 ||
			TargetKeyNode != TargetNode)
		{
			monsterAIController->SetPath(target->GetActorLocation());
			if(monsterAIController->GetPath().Num() > 0)
			{
				targetIndex = 0;
				currentWaypoint = monsterAIController->GetPath()[targetIndex];
			}
		}

		if(monsterAIController->GetPath().Num() > 0)
		{
			if(monsterAIController->GetGridActor()->NodeFromWorldPoint(currentWaypoint) ==
				monsterAIController->GetGridActor()->NodeFromWorldPoint(ControlledPawn->GetActorLocation()))
			{
				targetIndex++;
			
				if(!monsterAIController->GetPath().IsValidIndex(targetIndex))
				{
					targetIndex = monsterAIController->GetPath().Num() - 1;
					currentWaypoint = monsterAIController->GetPath().Last();
					monsterAIController->GetPath().Reset();
				}
				
				else
					currentWaypoint = monsterAIController->GetPath()[targetIndex];
			}
		
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMonster::PatrolPosKey, currentWaypoint);
			monsterAIController->SetCheckMoving(true);
			monsterAIController->MoveToLocation(currentWaypoint, -1, false, false);
			
			return EBTNodeResult::Succeeded;
		}
		
	}

	return EBTNodeResult::Failed;
}
