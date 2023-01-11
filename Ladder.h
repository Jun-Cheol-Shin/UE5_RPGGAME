// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Ladder.generated.h"


class UClimberComponent;
UCLASS()
class RPG_API ALadder : public AActor
{
	GENERATED_BODY()

private:

	bool bIsOnLadder = false;
	
	UPROPERTY(EditAnywhere, Category = "Ladder Options")
	float crossbarGap = 50.f;

	UPROPERTY(EditAnywhere, Category = "Ladder Options")
	float height = 300.f;
	

	UPROPERTY(EditAnywhere)
	USceneComponent* climberTransform;
	UPROPERTY(EditAnywhere)
	USceneComponent * DefaultSceneRoot;

	UPROPERTY(EditAnywhere)
	UInstancedStaticMeshComponent* crossBar;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ladder_Top;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* cylinder_L;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* cylinder_R;

	// 충돌 감지를 위한 박스 컴포넌트
	UPROPERTY(Category = Collision, VisibleAnywhere)
	UStaticMeshComponent* collisionBox;


	UPROPERTY()
	UClimberComponent* climbComp;
	
public:	
	// Sets default values for this actor's properties
	ALadder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;


	void AdjustSideBar(USceneComponent* comp) const;
	void InitLadder() const;


	UFUNCTION()
		void OnOverlapBegin( class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 OtherBodyIndex);

	USceneComponent* GetclimberTransform() { return climberTransform; }
	UStaticMeshComponent* GetLadderTop() { return ladder_Top; }

	float GetHeight() { return height; }

	bool GetLadderOn() const { return bIsOnLadder; }
	void SetLadderOn(bool _value) { bIsOnLadder = _value; }
};
