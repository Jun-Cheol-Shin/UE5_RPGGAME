// Fill out your copyright notice in the Description page of Project Settings.


#include "Pathfinding.h"
#include "GridActor.h"
#include "PathRequestManager.h"
#include "RPGCharacter.h"

// Sets default values for this component's properties
UPathfinding::UPathfinding()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	gridActor = nullptr;
	pathRequestManager = nullptr;
	// ...
}


// Called when the game starts
void UPathfinding::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if(!gridActor) gridActor = Cast<AGridActor>(GetOwner());
	if(!pathRequestManager) pathRequestManager = GetOwner()->FindComponentByClass<UPathRequestManager>();
}


// Called every frame
void UPathfinding::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FVector> UPathfinding::IsFindPath(FVector _start, FVector _dest, bool& _success)
{
	//UE_LOG(LogTemp, Error, TEXT("Find Path!!!"));
	TArray<FVector>	waypoints;
	//bool pathSuccess = false;
	//bool bEasyMove = true;
		
	FSTNode* startNode = gridActor->NodeFromWorldPoint(_start);
	FSTNode* targetNode = gridActor->NodeFromWorldPoint(_dest);
		
	if(startNode && targetNode && targetNode->bIsWalkable)
	{
		TArray<FSTNode*> openSet;
		TSet<FSTNode*> closeSet;
			
		openSet.Add(startNode);

		// 람다를 이용하여 포인터를 담은 배열을 정렬하기 위한 함수 선언
		auto pred = [](FSTNode const& _target1, FSTNode const& _target2)
		{
			if(_target1.fCost() == _target2.fCost())
			{
				return _target1.hCost < _target2.hCost;
			}
		
			return _target1.fCost() < _target2.fCost();
		};
			
		openSet.Add(startNode);

		// 배열을 최소힙구조로 전환
		openSet.Heapify(pred);
			
		while(!openSet.IsEmpty())
		{
			FSTNode* current = openSet.HeapTop();
			openSet.Remove(current);

			// 더이상 선형탐색이 필요없다.
			/*for(int i = 1; i < openSet.Num(); ++i)
			{
				if(openSet[i]->fCost() < current->fCost() ||
					openSet[i]->fCost() == current->fCost() && openSet[i]->hCost < current->hCost)
				{
					current = openSet[i];
				}
			}
			openSet.Remove(current);
			*/
				
			closeSet.Add(current);

			if(current == targetNode)
			{
				_success = true;
				break;
			}

			for (FSTNode* neighbour : gridActor->GetNeighbours(current))
			{
				if(!neighbour->bIsWalkable || closeSet.Contains(neighbour))
				{
					//bEasyMove = false;
					continue;
				}

				const int newMovementCostToNeighbour = current->gCost + GetDistance(current, neighbour);
				if(newMovementCostToNeighbour < neighbour->gCost || !openSet.Contains(neighbour))
				{
					neighbour->Set_gCost(newMovementCostToNeighbour);
					neighbour->Set_hCost(GetDistance(neighbour, targetNode));
					neighbour->parent = current;
				}

				if(!openSet.Contains(neighbour))
				{
					openSet.HeapPush(neighbour, pred);
				}
			}
		}
		
		if(_success)
		{
			/*if(bEasyMove)
			{
				waypoints.Add(_dest);
			}
			else
			{
				waypoints = RetracePath(startNode, targetNode);
			}*/

			waypoints = RetracePath(startNode, targetNode);
		}
	}
	
	return waypoints;
}

void UPathfinding::FindPath(FVector _startPos, FVector _targetPos)
{

	TArray<FVector> waypoints;
	bool pathSuccess = false;
	
	if(!gridActor) gridActor = Cast<AGridActor>(GetOwner());
	if(gridActor)
	{
		waypoints = IsFindPath(_startPos, _targetPos, pathSuccess);
		
		if(pathRequestManager)
		{
			pathRequestManager->FinishedProcessingPath(waypoints, pathSuccess);
		}
		
	}
}

TArray<FVector> UPathfinding::RetracePath(FSTNode* _start, FSTNode* _end)
{
	TArray<FSTNode*> path;
	FSTNode* current = _end;

	while(current != _start)
	{
		path.Add(current);
		current = current->parent;
	}

	
	TArray<FVector> waypoints = SimplifyPath(path);
	Algo::Reverse(waypoints);

	//if(gridActor) gridActor->SetPath(path);
	return waypoints;
}

TArray<FVector> UPathfinding::SimplifyPath(const TArray<FSTNode*>& _path)
{
	TArray<FVector> waypoints;

	for(const auto Iter : _path)
	{
		waypoints.Add(Iter->worldPosition);
	}
	
	/*FVector directionOld = FVector::Zero();

	for(int i = 1; i < _path.Num(); ++i)
	{
		FVector directionNew = FVector(_path[i - 1]->gridY - _path[i]->gridY,_path[i - 1]->gridX - _path[i]->gridX, 0);

		if(!directionNew.Equals(directionOld))
		{
			waypoints.Add(_path[i]->worldPosition);
		}

		directionOld = directionNew;
	}*/

	return waypoints;
}

int UPathfinding::GetDistance(FSTNode* _A, FSTNode* _B)
{
	int distX = abs(_A->gridX - _B->gridX);
	int distY = abs(_A->gridY - _B->gridY);

	if(distX > distY) return 14 * distY + 10 * (distX - distY);
	
	return 14 * distX + 10 * (distY - distX);
}



void UPathfinding::StartFindPath(FVector _start, FVector _target)
{
	FindPath(_start, _target);
}

