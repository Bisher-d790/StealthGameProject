// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameState.h"
#include "FPSPlayerController.h"
#include "Engine/World.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPlayer, bool bMissionSuccess) {


	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++) {
		AFPSPlayerController* PC = Cast<AFPSPlayerController>(It->Get());

		if (PC) {
			PC->OnMissionComplete(InstigatorPlayer, bMissionSuccess);

			// Disable Player Input
			APawn* Player = PC->GetPawn();
			if (Player) {
				Player->DisableInput(nullptr);
			}
		}
	}

}