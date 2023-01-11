// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterActor.h"
#include "Monster.h"
#include "RPGAIAnimInstance.h"

// Sets default values
AMonsterActor::AMonsterActor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AMonster::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AMonsterActor::BeginPlay()
{
	Super::BeginPlay();

	// Animation Tree Setting...
	UAnimInstance* Instance = GetMesh()->GetAnimInstance();

	if (Instance)
	{
		animInstance = Cast<URPGAIAnimInstance>(Instance);

		if (animInstance)
		{
			animInstance->SetMonster(Cast<AMonster>(this->GetController()));
			//UE_LOG(LogTemp, Warning, TEXT("Get Player!"));
		}
	}
}

// Called every frame
void AMonsterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


