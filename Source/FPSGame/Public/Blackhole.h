// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blackhole.generated.h"

UCLASS()
class FPSGAME_API ABlackhole : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlackhole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Pull Sphere")
		class USphereComponent* PullCollision;

	UPROPERTY(EditAnywhere, Category = "Pull Sphere")
		float PullRadius = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Pull Sphere")
		float PullStrength = -2000.f;

	UFUNCTION()
		void PullActors();

	UPROPERTY(EditAnywhere, Category = "Suction Sphere")
		class USphereComponent* InnerSphere;

	UPROPERTY(EditAnywhere, Category = "Suction Sphere")
		float InnerRadius = 100.f;

	UFUNCTION()
		void InnerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
