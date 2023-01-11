// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Pathfinding.generated.h"


class ARPGCharacter;
struct FSTNode;
class UPathRequestManager;
class AGridActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UPathfinding : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	AGridActor* gridActor;

	UPROPERTY()
	UPathRequestManager* pathRequestManager;

public:	
	// Sets default values for this component's properties
	UPathfinding();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<FVector> IsFindPath(FVector _start, FVector _dest, bool& _success);
	void FindPath(FVector _startPos, FVector _targetPos);
	int GetDistance(FSTNode* _A, FSTNode* _B);
	TArray<FVector> RetracePath(FSTNode* _start, FSTNode* _end);
	TArray<FVector> SimplifyPath(const TArray<FSTNode*>& _path);
	
	void StartFindPath(FVector _start, FVector _target);
};
