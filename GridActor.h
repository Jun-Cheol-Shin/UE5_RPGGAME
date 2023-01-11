// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "GameFramework/Actor.h"
#include "NodeActor.h"
#include "GridActor.generated.h"

USTRUCT()
struct F2DArray
{
	GENERATED_USTRUCT_BODY()
public:
	TArray<FSTNode*> array;

	F2DArray()
	{
		array.Reserve(1000);
	}
};

class UPathRequestManager;
class UPathfinding;
class ARPGCharacter;

UCLASS()
class RPG_API AGridActor : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere, Category = "Grid")
		UStaticMeshComponent* gridFloor;
	
	UPROPERTY(EditAnywhere, Category = "Grid")
		bool bDisplayGridGizmos = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Pathfinding")
		class UPathfinding* PathfindingComponent;
	UPROPERTY(VisibleAnywhere, Category = "Pathfinding")
		class UPathRequestManager* RequestManagerComponent;
	
	UPROPERTY(EditAnywhere, Category = "Grid")
		FVector2D gridWorldSize;
	UPROPERTY(EditAnywhere, Category = "Grid")
		float nodeSize = 50.f;

	UPROPERTY()
		TArray<F2DArray> grid;
	
	TArray<FSTNode*> path;
	
	FVector gridLocation;
	FVector2D gridSize;

	FVector firstNodeLocation;
	
public:
	UPROPERTY(EditAnywhere)
		USceneComponent* G_Scene;
	UPROPERTY(EditAnywhere)
		UBillboardComponent* G_Billboard;
	
	// Sets default values for this actor's properties
	AGridActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	void CreateGrid();

	void DrawGrid();

	
	FSTNode* NodeFromWorldPoint(const FVector& worldPos);
	FVector gridBottomLeft() const;
	FVector2D gridTileNumber() const;
	bool mySphereTrace(const FVector& Location, const float& size, const ECollisionChannel& type, FHitResult& result) const;

	TArray<FSTNode*> GetNeighbours(FSTNode* node);

	void SetPath(const TArray<FSTNode*>& _path) { path = _path; }
	TArray<FSTNode*>& GetPath() { return path; }

	UPathRequestManager* GetRequestManager() { return RequestManagerComponent; }
	UPathfinding* GetPathfindComp() { return PathfindingComponent; }
};
