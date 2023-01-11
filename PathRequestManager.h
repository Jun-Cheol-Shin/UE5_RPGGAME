// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathRequestManager.generated.h"


class UPathfinding;

DECLARE_DELEGATE(FCall_Back); 
DECLARE_DELEGATE_TwoParams(FCall_Back_Two_Params, const TArray<FVector>&, bool);

USTRUCT(Atomic, BlueprintType)
struct FSTPathRequest
{
	GENERATED_BODY()
public:
	FVector pathStart;
	FVector pathEnd;
	FCall_Back_Two_Params callBack;

	FSTPathRequest() = default;
	FSTPathRequest(const FVector& _start, const FVector& _end, const FCall_Back_Two_Params& _callBack) : pathStart(_start), pathEnd(_end), callBack(_callBack) {};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UPathRequestManager : public UActorComponent
{
	GENERATED_BODY()

private:
	TQueue<FSTPathRequest*> pathRequestQueue;
	FSTPathRequest* currentPathRequest;
	bool bIsProcessingPath = false;

	UPROPERTY()
	UPathfinding* pathfindingComponent;

protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UPathRequestManager();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
	void RequestPath(FVector pathStart, FVector pathEnd, const FCall_Back_Two_Params& callback);
	void TryProcessNext();
	void FinishedProcessingPath(TArray<FVector>& path, bool success);
};
