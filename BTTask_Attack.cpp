// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "RPGCharacter.h"
#include "Monster.h"
#include "MonsterActor.h"


UBTTask_Attack::UBTTask_Attack()
{
	//틱기능 활성화. Finish를 계속 체크한다.
	bNotifyTick = true;
	//아직 공격중이 아니다
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto monster = Cast<AMonsterActor>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == monster)
		return EBTNodeResult::Failed;

	//ABCharacter->Attack();
	//IsAttacking = true;
	//람다식.ABCharacter이 AttackEnd Delegate를 호출하면 IsAttacking을 false로

	/*ABCharacter->OnAttackEnd.AddLambda([this]()->void {
		IsAttacking = false;
	});*/

	//일단 InProgress에서 머물게한다.공격이 끝나기 전까지 계속 지연시켜준다.
	return EBTNodeResult::InProgress;
}

//매 틱마다 공격태스크가 끝났는지를 체크한다.
void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	//OnAttackEnd Delegate가 호출되면 Attacking이 false 되므로
	if (!IsAttacking)
	{
		//공격 태스크가 끝났음을 알려준다
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}