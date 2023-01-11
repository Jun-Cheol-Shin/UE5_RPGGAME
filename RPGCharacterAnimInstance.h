// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RPGCharacterAnimInstance.generated.h"

/**
 * 
 */
class ARPGCharacter;
UCLASS()
class RPG_API URPGCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	UPROPERTY()
	ARPGCharacter* myChracter;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bRunning;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bClimbing;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCompleteClimbing;
	
public:
	void GetPlayer(ARPGCharacter* Player) { myChracter = Player; }
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
