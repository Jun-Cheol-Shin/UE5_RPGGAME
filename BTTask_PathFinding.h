// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PathFinding.generated.h"

/**
 * 
 */
class AMonster;
UCLASS()
class RPG_API UBTTask_PathFinding : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY()
	AMonster* monsterAIController;

	int targetIndex;
	FVector currentWaypoint;
	
public:
	UBTTask_PathFinding();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
