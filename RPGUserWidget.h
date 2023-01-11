// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HPBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* MPBar;
};
