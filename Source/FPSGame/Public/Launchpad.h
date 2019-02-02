// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Launchpad.generated.h"

UCLASS()
class FPSGAME_API ALaunchpad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaunchpad();

private:
	UPROPERTY(EditAnywhere, Category = "Components")
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Components")
		class UBoxComponent* OverlapComp;

	UPROPERTY(EditAnywhere, Category = "Launch Player")
		float PlayerLaunchPower = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Launch Player")
		UParticleSystem* PLaunchFX;

	UPROPERTY(EditAnywhere, Category = "Launch Object")
		float ObjectsLaunchPower = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Launch Object")
		UParticleSystem* OLaunchFX;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
