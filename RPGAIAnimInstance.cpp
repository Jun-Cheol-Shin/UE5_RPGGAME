// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAIAnimInstance.h"
#include "Monster.h"

void URPGAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(monster)
	{
		bRunning = monster->GetAIMoving();
	}
	
}
