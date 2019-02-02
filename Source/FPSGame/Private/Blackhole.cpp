// Fill out your copyright notice in the Description page of Project Settings.

#include "Blackhole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABlackhole::ABlackhole()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = Mesh;

	PullCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Outer Sphere Component"));
	PullCollision->InitSphereRadius(PullRadius);
	PullCollision->SetupAttachment(Mesh);

	InnerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Inner Sphere Component"));
	InnerSphere->InitSphereRadius(InnerRadius);
	InnerSphere->SetupAttachment(Mesh);

	InnerSphere->OnComponentBeginOverlap.AddDynamic(this,&ABlackhole::InnerOverlap);

}

// Called when the game starts or when spawned
void ABlackhole::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABlackhole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PullActors();
}

void ABlackhole::PullActors() {
	TArray<UPrimitiveComponent*> ActorsToPull;
	PullCollision->GetOverlappingComponents(ActorsToPull);

	for (int32 i = 0; i < ActorsToPull.Num(); i++) {
		UPrimitiveComponent* PrimComp = ActorsToPull[i];
		if (PrimComp && PrimComp->IsSimulatingPhysics()) {
			PrimComp->AddRadialForce(GetActorLocation(), PullCollision->GetScaledSphereRadius(), this->PullStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

void ABlackhole::InnerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		OtherActor->Destroy();
	}

}