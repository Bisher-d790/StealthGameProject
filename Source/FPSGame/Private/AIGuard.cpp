// Fill out your copyright notice in the Description page of Project Settings.

#include "AIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "FPSGameMode.h"
#include "Engine/World.h"
#include "FPSCharacter.h"
#include "Net/UnrealNetwork.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>

// Sets default values
AAIGuard::AAIGuard()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));


	PawnSensingComp->OnSeePawn.AddDynamic(this, &AAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AAIGuard::OnNoiseHeard);

	GuardState = EAIState::Idle;


}

// Called when the game starts or when spawned
void AAIGuard::BeginPlay()
{
	Super::BeginPlay();

	originalRotation = GetActorRotation();

	if (bMoveAI) {
		MoveAI();
	}
}


// Called every frame
void AAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentWaypoint) {
		FVector Delta = GetActorLocation() - CurrentWaypoint->GetActorLocation();
		float distance = Delta.Size();
		
		if (distance <= DistanceToReachWaypoint) {
			MoveAI();
		}
	}
}


void AAIGuard::ResetActorRotation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	SetActorRotation(originalRotation);

	SetGuardState(EAIState::Idle);

	if (bMoveAI) {
		MoveAI();
	}
}

void AAIGuard::OnPawnSeen(APawn* Pawn)
{
	class AFPSCharacter* SeenPlayer = Cast<AFPSCharacter>(Pawn);
	if (SeenPlayer == nullptr)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), SeenPlayer->GetActorLocation(), 32.f, 12.f, FColor::Red, false, 10.f);

	AFPSGameMode* gamemode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (gamemode)
	{
		gamemode->MissionComplete(SeenPlayer, false);
	}

	SetGuardState(EAIState::Alerted);

	AController* Controller = GetController();
	if (Controller)
		Controller->StopMovement();
}

void AAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	class AFPSCharacter* HeardPlayer = Cast<AFPSCharacter>(NoiseInstigator);

	DrawDebugSphere(GetWorld(), Location, 32.f, 12.f, FColor::Green, false, 10.f);


	FRotator newActorRotation = FRotationMatrix::MakeFromX(Location - GetActorLocation()).Rotator();
	newActorRotation.Roll = 0.f;
	newActorRotation.Pitch = 0.f;

	SetActorRotation(newActorRotation);

	GetWorldTimerManager().ClearTimer(SetActorRotationHandle);
	GetWorldTimerManager().SetTimer(SetActorRotationHandle, this, &AAIGuard::ResetActorRotation, 3.f);

	SetGuardState(EAIState::Suspicious);

	AController* Controller = GetController();
	if (Controller)
		Controller->StopMovement();	 
}

void AAIGuard::OnRep_GuardState()
{
	OnGuardStateChange(GuardState);
}

void AAIGuard::SetGuardState(EAIState newState)
{
	if (GuardState == newState)
	{
		return;
	}

	GuardState = newState;
	OnRep_GuardState();
}

void AAIGuard::MoveAI()
{
	if (CurrentWaypoint == nullptr || CurrentWaypointIndex == (Waypoints.Num()-1)) {
		CurrentWaypoint = Waypoints[0];
		CurrentWaypointIndex = 0;
	}
	else {
		CurrentWaypoint = Waypoints[++CurrentWaypointIndex];
	}
	
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentWaypoint);
}

void AAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAIGuard, GuardState);
}