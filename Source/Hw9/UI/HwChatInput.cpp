// Fill out your copyright notice in the Description page of Project Settings.


#include "HwChatInput.h"

#include "Components/EditableTextBox.h"
#include "Hw9/Player/HwPlayerController.h"

void UHwChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (TextBoxChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		TextBoxChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UHwChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (TextBoxChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		TextBoxChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UHwChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		AHwPlayerController* OwningCXPlayerController = Cast<AHwPlayerController>(GetOwningPlayer());
		if (IsValid(OwningCXPlayerController))
		{
			OwningCXPlayerController->SetChatMessageString(Text.ToString());

			TextBoxChatInput->SetText(FText());
		}
	}
}
