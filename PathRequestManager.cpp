// Fill out your copyright notice in the Description page of Project Settings.


#include "PathRequestManager.h"

#include "Pathfinding.h"

// Sets default values for this component's properties
UPathRequestManager::UPathRequestManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPathRequestManager::BeginPlay()
{
	Super::BeginPlay();

	// ...

	bIsProcessingPath = false;
	if(!pathfindingComponent) pathfindingComponent = GetOwner()->FindComponentByClass<UPathfinding>();
}


// Called every frame
void UPathRequestManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPathRequestManager::RequestPath(FVector pathStart, FVector pathEnd, const FCall_Back_Two_Params& callback)
{
	FSTPathRequest* newRequest = new FSTPathRequest(pathStart, pathEnd, callback);
	pathRequestQueue.Enqueue(newRequest);
	TryProcessNext();
}

void UPathRequestManager::TryProcessNext()
{
	if(!bIsProcessingPath && !pathRequestQueue.IsEmpty())
	{
		pathRequestQueue.Dequeue(currentPathRequest);
		bIsProcessingPath = true;
		if(pathfindingComponent) pathfindingComponent->FindPath(currentPathRequest->pathStart, currentPathRequest->pathEnd);
	}
}

void UPathRequestManager::FinishedProcessingPath(TArray<FVector>& path, bool success)
{
	if(currentPathRequest->callBack.IsBound())
	{
		//UE_LOG(LogTemp, Error, TEXT("Execute!!"));
		currentPathRequest->callBack.Execute(path, success);
	}
	
	bIsProcessingPath = false;
	TryProcessNext();
}

