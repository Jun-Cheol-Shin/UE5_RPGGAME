// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Climber.h"
#include "ClimberComponent.generated.h"

class ALadder;
class ARPGCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UClimberComponent : public UActorComponent,  public IClimber
{
	GENERATED_BODY()

private:
	bool bOnLadder;
	bool bCompleteLadder;
	float tempLocationZ;
	
	UPROPERTY()
	ALadder* ladderActor;
	UPROPERTY()
	ARPGCharacter* rpgChar;

	void Climbing();

public:	
	// Sets default values for this component's properties
	UClimberComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SetLadder(ALadder* _ladder) override;

	bool GetLadderClimb() const { return bOnLadder; }
	bool GetCompLadderClimb() const { return bCompleteLadder; }
	void InterRaction();
};
