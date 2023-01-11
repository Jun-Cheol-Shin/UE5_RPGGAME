// Fill out your copyright notice in the Description page of Project Settings.
#include "ClimberComponent.h"
#include "Ladder.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RPGCharacter.h"

void UClimberComponent::Climbing()
{
	if(rpgChar && bOnLadder && ladderActor)
	{
		if(ladderActor->GetLadderOn())
		{
			FVector dir = (ladderActor->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
			if(FMath::IsNearlyZero(tempLocationZ - ladderActor->GetHeight() - GetOwner()->GetActorLocation().Z, 5.f))
			{
				rpgChar->SetGridActor();
				UE_LOG(LogTemp, Error, TEXT("Arrive!!! under ladder"));
				rpgChar->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
				bCompleteLadder = false;
				bOnLadder = false;
				ladderActor->SetLadderOn(false);
			}
			else
			{
				rpgChar->AddMovementInput(dir, 0.3f);
			}
		}

		else
		{
			FVector dir = (ladderActor->GetLadderTop()->GetComponentLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();

			if(bCompleteLadder)
			{
				rpgChar->AddMovementInput(GetOwner()->GetActorForwardVector() - GetOwner()->GetActorUpVector() * 0.14f, 0.25f);
			}
			
			else if(ladderActor->GetLadderTop()->GetComponentLocation().Z <= GetOwner()->GetActorLocation().Z)
			{
				//GetCharacterMovement()->StopMovementImmediately();
				UE_LOG(LogTemp, Error, TEXT("Arrive!!"));
				bCompleteLadder = true;
				FTimerHandle timer;
			
				GetWorld()->GetTimerManager().SetTimer(timer, [this]()
				{
					rpgChar->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
					rpgChar->SetGridActor();
					bCompleteLadder = false;
					bOnLadder = false;

					if(ladderActor)
					{
						ladderActor->SetLadderOn(true);
						tempLocationZ = GetOwner()->GetActorLocation().Z;
					}
				
				},1.5f, false);
			}

			else
			{
				rpgChar->AddMovementInput(dir, 0.2f);
			}
		}
	}
}



// Sets default values for this component's properties
UClimberComponent::UClimberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	ladderActor = nullptr;
	bOnLadder = false;
	bCompleteLadder = false;
}


// Called when the game starts
void UClimberComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...

	rpgChar = Cast<ARPGCharacter>(GetOwner());
}


// Called every frame
void UClimberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Climbing();
	// ...
}


void UClimberComponent::SetLadder(ALadder* _ladder)
{
	IClimber::SetLadder(_ladder);
	ladderActor = _ladder;
}

void UClimberComponent::InterRaction()
{
	if(ladderActor && !bOnLadder)
	{
		bOnLadder = true;
		rpgChar->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		FLatentActionInfo Info;
		Info.CallbackTarget = this;

		if(ladderActor->GetclimberTransform())
		{
			FRotator HangingRot = FRotator(0,-ladderActor->GetActorRotation().Yaw,0);
			FVector HangingVec = FVector(ladderActor->GetclimberTransform()->GetComponentLocation().X,
				ladderActor->GetclimberTransform()->GetComponentLocation().Y,
				GetOwner()->GetActorLocation().Z);
			//SetActorLocation(HangingVec);
			UKismetSystemLibrary::MoveComponentTo(GetOwner()->GetRootComponent(), HangingVec, HangingRot,
				false, false, 0.2f, false,EMoveComponentAction::Type::Move, Info);
		}
	}
}
