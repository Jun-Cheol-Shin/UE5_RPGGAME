// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPos.generated.h"

/**
 * 
 */
class AMonster;
UCLASS()
class RPG_API UBTTask_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY()
	AMonster* monsterAIController;

public:
	UBTTask_FindPatrolPos();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
