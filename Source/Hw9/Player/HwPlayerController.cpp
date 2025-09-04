// Fill out your copyright notice in the Description page of Project Settings.


#include "HwPlayerController.h"

#include "EngineUtils.h"
#include "HwPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Hw9/Hw9.h"
#include "Hw9/GameModes/HwGameModeBase.h"
#include "Hw9/UI/HwChatInput.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AHwPlayerController::AHwPlayerController()
{
	bReplicates = true;
}

void AHwPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void AHwPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	/*FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);*/

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UHwChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();

			if (IsValid(ChatInputWidgetInstance->TextBoxChatInput))
			{
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(ChatInputWidgetInstance->TextBoxChatInput->TakeWidget());

				SetInputMode(InputMode);

				bShowMouseCursor = true;
			}
		}
	}

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void AHwPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void AHwPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		AHwGameModeBase* CXGM = Cast<AHwGameModeBase>(GM);
		if (IsValid(CXGM) == true)
		{
			CXGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

void AHwPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		AHwPlayerState* HwPlayerState = GetPlayerState<AHwPlayerState>();
		if (IsValid(HwPlayerState) == true)
		{
			FString CombinedMessageString = HwPlayerState->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void AHwPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	HwFunctionLibrary::HwPrintString(this, InChatMessageString, 10.f);
}
