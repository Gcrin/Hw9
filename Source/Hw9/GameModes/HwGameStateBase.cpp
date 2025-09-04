// Fill out your copyright notice in the Description page of Project Settings.


#include "HwGameStateBase.h"

#include "Hw9/Player/HwPlayerController.h"
#include "Kismet/GameplayStatics.h"

void AHwGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			AHwPlayerController* HwPC = Cast<AHwPlayerController>(PC);
			if (IsValid(HwPC) == true)
			{
				FString NotificationString = FString::Printf(TEXT("%s님이 게임에 입장했습니다."), *InNameString);
				HwPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
