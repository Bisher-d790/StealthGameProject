// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "kismet/GameplayStatics.h"
#include "FPSGameState.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::MissionComplete(APawn* InstigatorPlayer, bool bMissionSuccess)
{
	if (InstigatorPlayer) {

		TArray<AActor*> ViewTargets;
		UGameplayStatics::GetAllActorsOfClass(this, SpectatingClass, ViewTargets);
		AActor* NewViewtarget = ViewTargets[0];

		APlayerController* PC = Cast<APlayerController>(InstigatorPlayer->GetController());
		if (PC) {
			PC->SetViewTargetWithBlend(NewViewtarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
		}

		AFPSGameState* GS = GetGameState<AFPSGameState>();
		if (GS) {
			GS->MulticastOnMissionComplete(InstigatorPlayer, bMissionSuccess);
		}

		OnMissionComplete(InstigatorPlayer, bMissionSuccess);
	}
}
