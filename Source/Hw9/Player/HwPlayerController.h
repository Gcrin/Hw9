// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HwPlayerController.generated.h"

class UHwChatInput;
/**
 * 
 */
UCLASS()
class HW9_API AHwPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHwPlayerController();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

	void SetChatMessageString(const FString& InChatMessageString);
	void PrintChatMessageString(const FString& InChatMessageString);

	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHwChatInput> ChatInputWidgetClass;
	UPROPERTY()
	TObjectPtr<UHwChatInput> ChatInputWidgetInstance;

	FString ChatMessageString;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;
};
