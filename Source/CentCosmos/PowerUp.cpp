// Fill out your copyright notice in the Description page of Project Settings.
#include "PowerUp.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CentCosmosPawn.h"

APowerUp::APowerUp()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CajaColision"));
	RootComponent = CollisionBox;
	CollisionBox->SetBoxExtent(FVector(60.f, 60.f, 60.f));
	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->SetCollisionProfileName(TEXT("Trigger"));

	MallaVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaVisual"));
	MallaVisual->SetupAttachment(RootComponent);
	MallaVisual->SetCollisionProfileName(TEXT("NoCollision"));
}

void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APowerUp::OnOverlap);
	}
}

void APowerUp::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		ACentCosmosPawn* Nave = Cast<ACentCosmosPawn>(OtherActor);
		if (Nave)
		{
			AplicarEfecto(Nave);
			Destroy();
		}
	}
}

void APowerUp::AplicarEfecto(ACentCosmosPawn* Nave)
{
	
}