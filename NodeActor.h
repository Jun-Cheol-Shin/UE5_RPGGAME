// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NodeActor.generated.h"

UCLASS()
class RPG_API ANodeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANodeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

USTRUCT(Atomic, BlueprintType)
struct FSTNode
{
	GENERATED_BODY()
public:
	FSTNode* parent;
	UPROPERTY(EditAnywhere)
		bool bIsWalkable = false;
	UPROPERTY(EditAnywhere)
		FVector worldPosition;

	// Grid Axis
	int gridX;
	int gridY;

	// Cost Value
	int gCost;
	int hCost;
	
	bool operator()(const FSTNode& _target1, const FSTNode& _target2) const
	{
		if(_target1.fCost() == _target2.fCost())
		{
			if(_target1.hCost == _target2.hCost)
			{
				return _target1.gCost > _target2.gCost;
			}
			
			return _target1.hCost < _target2.hCost;
		}
	
		return _target1.fCost() < _target2.fCost();
	}

	bool operator==(const FSTNode& _target)
	{
		if(gridX == _target.gridX && gridY == _target.gridY) return true;
		return false;
	}

	FSTNode() = default;
	FSTNode(const bool& _walk, const FVector& _pos, const int& _gridX, const int& _gridY) : bIsWalkable(_walk), worldPosition(_pos), gridX(_gridX), gridY(_gridY) {};

	~FSTNode() = default;

	int fCost() const
	{
		return gCost + hCost;
	}

	void Set_gCost(int _cost) { gCost = _cost; }
	void Set_hCost(int _cost) { hCost = _cost; }
};