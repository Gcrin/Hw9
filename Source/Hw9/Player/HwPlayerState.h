// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HwPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HW9_API AHwPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AHwPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FString GetPlayerInfoString();

	UPROPERTY(Replicated)
	FString PlayerNameString = TEXT("None");

	UPROPERTY(Replicated)
	int32 CurrentGuessCount = 0;

	UPROPERTY(Replicated)
	int32 MaxGuessCount = 3;
};
