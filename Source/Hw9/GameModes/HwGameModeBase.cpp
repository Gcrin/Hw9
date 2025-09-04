// Fill out your copyright notice in the Description page of Project Settings.


#include "HwGameModeBase.h"

#include "EngineUtils.h"
#include "HwGameStateBase.h"
#include "Hw9/Player/HwPlayerController.h"
#include "Hw9/Player/HwPlayerState.h"

void AHwGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AHwPlayerController* HwPlayerController = Cast<AHwPlayerController>(NewPlayer);
	if (IsValid(HwPlayerController) == true)
	{
		HwPlayerController->NotificationText = FText::FromString(TEXT("게임 서버에 연결되었습니다."));
		AllPlayerControllers.Add(HwPlayerController);

		AHwPlayerState* HwPlayerState = HwPlayerController->GetPlayerState<AHwPlayerState>();
		if (IsValid(HwPlayerState) == true)
		{
			HwPlayerState->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		AHwGameStateBase* HwGameStateBase =  GetGameState<AHwGameStateBase>();
		if (IsValid(HwGameStateBase) == true)
		{
			HwGameStateBase->MulticastRPCBroadcastLoginMessage(HwPlayerState->PlayerNameString);
		}
	}
}

void AHwGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
}

void AHwGameModeBase::PrintChatMessageString(AHwPlayerController* InChattingPlayerController,
	const FString& InChatMessageString)
{
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);

		IncreaseGuessCount(InChattingPlayerController);
		
		for (TActorIterator<AHwPlayerController> It(GetWorld()); It; ++It)
		{
			AHwPlayerController* HwPlayerController = *It;
			if (IsValid(HwPlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				HwPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<AHwPlayerController> It(GetWorld()); It; ++It)
		{
			AHwPlayerController* HwPlayerController = *It;
			if (IsValid(HwPlayerController) == true)
			{
				HwPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

FString AHwGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });
	
	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool AHwGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}
			
			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;
		
	} while (false);	

	return bCanPlay;
}

FString AHwGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else 
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;				
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void AHwGameModeBase::IncreaseGuessCount(AHwPlayerController* InChattingPlayerController)
{
	AHwPlayerState* HwPS = InChattingPlayerController->GetPlayerState<AHwPlayerState>();
	if (IsValid(HwPS) == true)
	{
		HwPS->CurrentGuessCount++;
	}
}

void AHwGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& HwPlayerController : AllPlayerControllers)
	{
		AHwPlayerState* HwPS = HwPlayerController->GetPlayerState<AHwPlayerState>();
		if (IsValid(HwPS) == true)
		{
			HwPS->CurrentGuessCount = 0;
		}
	}
}

void AHwGameModeBase::JudgeGame(AHwPlayerController* InChattingPlayerController, int InStrikeCount)
{if (3 == InStrikeCount)
{
	AHwPlayerState* HwPS = InChattingPlayerController->GetPlayerState<AHwPlayerState>();
	for (const auto& HwPlayerController : AllPlayerControllers)
	{
		if (IsValid(HwPS) == true)
		{
			FString CombinedMessageString = HwPS->PlayerNameString + TEXT(" has won the game.");
			HwPlayerController->NotificationText = FText::FromString(CombinedMessageString);

			ResetGame();
		}
	}
}
else
{
	bool bIsDraw = true;
	for (const auto& HwPlayerController : AllPlayerControllers)
	{
		AHwPlayerState* HwPS = HwPlayerController->GetPlayerState<AHwPlayerState>();
		if (IsValid(HwPS) == true)
		{
			if (HwPS->CurrentGuessCount < HwPS->MaxGuessCount)
			{
				bIsDraw = false;
				break;
			}
		}
	}

	if (true == bIsDraw)
	{
		for (const auto& HwPlayerController : AllPlayerControllers)
		{
			HwPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));

			ResetGame();
		}
	}
}
}
