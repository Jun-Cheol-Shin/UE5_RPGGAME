// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterActor.generated.h"


class URPGAIAnimInstance;
UCLASS()
class RPG_API AMonsterActor : public ACharacter
{
	GENERATED_BODY()

private:

public:
	// Sets default values for this character's properties
	AMonsterActor();

protected:

	UPROPERTY()
	URPGAIAnimInstance* animInstance;

	UPROPERTY(EditAnywhere, Category = "AIRange")
	float fAttackRange = 100.f;

	UPROPERTY(EditAnywhere, Category = "AIRange")
	float fDetectRange = 600.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	float GetAttackRange() const { return fAttackRange; }
	float GetDetectRange() const { return fDetectRange; }
	
};
