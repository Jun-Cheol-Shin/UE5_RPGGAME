// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacterAnimInstance.h"
#include "ClimberComponent.h"
#include "RPGCharacter.h"

void URPGCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(myChracter)
	{
		bRunning = myChracter->GetRunning();
		//bClimbing = myChracter->GetClimbComp()->GetLadderClimb();
		//bCompleteClimbing = myChracter->GetClimbComp()->GetCompLadderClimb();
	}
}
