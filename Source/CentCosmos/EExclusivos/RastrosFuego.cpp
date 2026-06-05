// Fill out your copyright notice in the Description page of Project Settings.

#include "RastrosFuego.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../Player/CentCosmosPawn.h"
#include "Engine/World.h"
#include "TimerManager.h"

ARastrosFuego::ARastrosFuego()
{
	PrimaryActorTick.bCanEverTick = false;
	FuegoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FuegoMesh"));
	RootComponent = FuegoMesh;

	FuegoMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FuegoMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	if (PlaneMesh.Succeeded()) FuegoMesh->SetStaticMesh(PlaneMesh.Object);

	JugadorEnFuego = nullptr;
}

void ARastrosFuego::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(4.0f);
}

void ARastrosFuego::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
	{
		JugadorEnFuego = Cast<ACentCosmosPawn>(OtherActor);
		// Inicia el loop de daño: Daño inmediato y luego cada 2 segundos
		GetWorld()->GetTimerManager().SetTimer(TimerDanoContinuo, this, &ARastrosFuego::AplicarDanoContinuo, 2.0f, true, 0.0f);
	}
}

void ARastrosFuego::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (OtherActor && OtherActor == JugadorEnFuego)
	{
		// Si sale del fuego, cancelamos el timer
		GetWorld()->GetTimerManager().ClearTimer(TimerDanoContinuo);
		JugadorEnFuego = nullptr;
	}
}

void ARastrosFuego::AplicarDanoContinuo()
{
	if (JugadorEnFuego)
	{
		JugadorEnFuego->RecibirDanioNave(3.0f); // DAÑO CONSTANTE SEGÚN LISTA
	}
}

