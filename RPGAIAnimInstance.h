// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RPGAIAnimInstance.generated.h"

/**
 * 
 */
class AMonster;
UCLASS()
class RPG_API URPGAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	UPROPERTY()
	AMonster* monster;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bRunning;

public:
	void SetMonster(AMonster* _actor)  { monster = _actor; }
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
