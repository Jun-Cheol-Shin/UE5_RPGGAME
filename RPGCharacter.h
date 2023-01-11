// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"

#include "RPGCharacter.generated.h"

class UPathRequestManager;
class AGridActor;
class URPGCharacterAnimInstance;
class UClimberComponent;

UCLASS()
class RPG_API ARPGCharacter : public ACharacter
{
	GENERATED_BODY()
	
private:
	
	FVector currentWaypoint;
	FTimerHandle moveTimer;
	TArray<FVector> path;

	UPROPERTY(EditAnywhere, Category = "Camera")
	bool bIsQuarterView = false;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* springArm;
	UPROPERTY(EditAnywhere)
	UCameraComponent* quarterViewCamera;

	float tempLocationZ;
	FVector DestLocation;
	int targetIndex;
	
public:
	// Sets default values for this character's properties
	ARPGCharacter();

protected:
	bool bClickMouse;
	bool bIsStartMove = false;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	UPROPERTY()
	URPGCharacterAnimInstance* animInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class URPGUserWidget> PlayerUMGClass;
	UPROPERTY()
	URPGUserWidget* PlayerUMG;
	UPROPERTY()
	AGridActor* gridActor;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Events..
	void InputClickPressed();
	void InputClickReleased();
	void ZoomInCamera();
	void ZoomOutCamera();

	UFUNCTION()
	void OnPathFound(const TArray<FVector>& newPath, bool pathSuccessful);
	
	bool GetDestinationToMove();
	void SetGridActor();
	
	void FollowPath(float DeltaTime);
	void CharacterMove();


	// Properties....
	const TArray<FVector>& GetPlayerPath() { return path; }
	void SetViewMode(bool _mode) { bIsQuarterView = _mode; }
	bool GetRunning() const { return bIsStartMove; }
	//UClimberComponent* GetClimbComp() const { return ClimberComponent; }
	
};
