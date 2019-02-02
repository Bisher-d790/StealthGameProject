// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIGuard.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class FPSGAME_API AAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UPawnSensingComponent* PawnSensingComp;


	UFUNCTION(BlueprintCallable)
		void OnPawnSeen(APawn* Pawn);

	UFUNCTION(BlueprintCallable)
		void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	FTimerHandle SetActorRotationHandle;
	FRotator originalRotation;

	UPROPERTY(ReplicatedUsing = OnRep_GuardState)
		EAIState GuardState;

	UFUNCTION()
		void OnRep_GuardState();

	void SetGuardState(EAIState newState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
		void OnGuardStateChange(EAIState newState);

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bMoveAI"))
		TArray<AActor*> Waypoints;

	int32 CurrentWaypointIndex;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bMoveAI"))
		float DistanceToReachWaypoint = 100.f;
	AActor* CurrentWaypoint = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "AI")
		bool bMoveAI = false;

	UFUNCTION()
		void MoveAI();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ResetActorRotation();
};
