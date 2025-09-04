// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HwGameModeBase.generated.h"

class AHwPlayerController;
/**
 * 
 */
UCLASS()
class HW9_API AHwGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void BeginPlay() override;
	
	void PrintChatMessageString(AHwPlayerController* InChattingPlayerController, const FString& InChatMessageString);
	FString GenerateSecretNumber();
	bool IsGuessNumberString(const FString& InNumberString);
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
	void IncreaseGuessCount(AHwPlayerController* InChattingPlayerController);
	void ResetGame();
	void JudgeGame(AHwPlayerController* InChattingPlayerController, int InStrikeCount);

protected:
	FString SecretNumberString;
	TArray<TObjectPtr<AHwPlayerController>> AllPlayerControllers;
};
