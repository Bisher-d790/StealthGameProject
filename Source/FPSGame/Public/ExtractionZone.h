// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExtractionZone.generated.h"

UCLASS()
class FPSGAME_API AExtractionZone : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExtractionZone();

private:
	UPROPERTY(EditAnywhere, Category = "Components")
		class UBoxComponent* OverlapComp;

	UFUNCTION(Category = "Collision")
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditAnywhere, Category = "Components")
		class UDecalComponent* DecalComp;

	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundBase* ObjectiveMissing;
};
