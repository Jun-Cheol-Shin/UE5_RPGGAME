// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "Monster.h"
#include "MonsterActor.h"
#include "RPGCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AMonsterActor* ControllingPawn = Cast<AMonsterActor>(OwnerComp.GetAIOwner()->GetPawn());

	if (nullptr == ControllingPawn)
		return false;

	auto Target = Cast<ARPGCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonster::TargetKey));
	if (nullptr == Target)
		return false;
	
	// magic number 작업 필요
	bResult = (Target->GetDistanceTo(ControllingPawn) <= ControllingPawn->GetAttackRange());
	return bResult;
}
