// Fill out your copyright notice in the Description page of Project Settings.


#include "HwPlayerController.h"

#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Hw9/Hw9.h"
#include "Hw9/UI/HwChatInput.h"
#include "Kismet/KismetSystemLibrary.h"

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
}

void AHwPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void AHwPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	for (TActorIterator<AHwPlayerController> It(GetWorld()); It; ++It)
	{
		AHwPlayerController* CXPlayerController = *It;
		if (IsValid(CXPlayerController) == true)
		{
			CXPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
		}
	}
}

void AHwPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		ServerRPCPrintChatMessageString(InChatMessageString);
	}
}

void AHwPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	HwFunctionLibrary::HwPrintString(this, InChatMessageString, 10.f);
}
