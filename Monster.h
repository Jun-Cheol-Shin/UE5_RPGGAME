// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Monster.generated.h"

/**
 * 
 */
class AGridActor;
class UPathfinding;

UCLASS()
class RPG_API AMonster : public AAIController
{
	GENERATED_BODY()

	// 
private:
	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BTAsset;
	UPROPERTY(EditAnywhere, Category = "AI")
	UBlackboardData* BBAsset;


	TArray<FVector> Path;

private:
	UPROPERTY()
	AGridActor* gridActor;

public:
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

public:
	AMonster();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void Tick(float DeltaTime) override;
	bool GetAIMoving() const { return bIsMoving; }


	AGridActor* GetGridActor() const { return gridActor; }

	void SetCheckMoving(bool _flag) { bCheckLocation = _flag; }
	bool GetCheckMoving() const { return bCheckLocation; }

	TArray<FVector> GetPath() const { return Path; }
	bool SetPath(const FVector& _dst);
	
private:

	// fields...
	bool bCheckLocation;
	bool bIsMoving;
	
	void SetGridActor();
};
