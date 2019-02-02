// Fill out your copyright notice in the Description page of Project Settings.

#include "Launchpad.h"
#include "components/StaticMeshComponent.h"
#include "components/BoxComponent.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALaunchpad::ALaunchpad()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = Mesh;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Component"));
	OverlapComp->InitBoxExtent(FVector(200.f, 200.f, 100.f));
	OverlapComp->SetupAttachment(RootComponent);
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ALaunchpad::BeginOverlap);

}

void ALaunchpad::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AFPSCharacter* player = Cast<AFPSCharacter>(OtherActor);
	if (player != nullptr) {
		player->LaunchCharacter((GetActorForwardVector()+FVector(0.f,0.f,1.f))*PlayerLaunchPower, true, true);
		UGameplayStatics::SpawnEmitterAtLocation(this, PLaunchFX, this->GetActorLocation());
	}
	else {
		if (OtherComp && OtherComp->IsSimulatingPhysics()) {
			OtherComp->AddImpulse((GetActorForwardVector() + FVector(0.f, 0.f, 1.f))*ObjectsLaunchPower, NAME_None, true);
			UGameplayStatics::SpawnEmitterAtLocation(this, OLaunchFX, this->GetActorLocation());
		}
	}
}

