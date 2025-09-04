// Fill out your copyright notice in the Description page of Project Settings.


#include "HwGameModeBase.h"

#include "HwGameStateBase.h"

void AHwGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AHwGameStateBase* HwGameStateBase = GetGameState<AHwGameStateBase>();
	if (IsValid(HwGameStateBase) == true)
	{
		HwGameStateBase->MulticastRPCBroadcastLoginMessage(NewPlayer->GetName());
	}
}
