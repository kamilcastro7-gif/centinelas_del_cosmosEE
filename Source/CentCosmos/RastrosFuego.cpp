// Fill out your copyright notice in the Description page of Project Settings.

#include "RastrosFuego.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "CentCosmosPawn.h"
#include "Engine/World.h"

ARastrosFuego::ARastrosFuego()
{
	PrimaryActorTick.bCanEverTick = false;

	FuegoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FuegoMesh"));
	RootComponent = FuegoMesh;

	FuegoMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FuegoMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// IMPLEMENTACIÓN: Malla del plano para el rastro de fuego en el suelo
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	if (PlaneMesh.Succeeded())
	{
		FuegoMesh->SetStaticMesh(PlaneMesh.Object);
	}

	DanoPorSegundo = 15.0f;
}

void ARastrosFuego::BeginPlay()
{
	Super::BeginPlay();

	// El rastro dura exactamente 4 segundos en el mapa
	SetLifeSpan(4.0f);
}

void ARastrosFuego::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
	{
		// Si tienes una función para restar vida en tu nave, la llamas aquí
		// Cast<ACentCosmosPawn>(OtherActor)->RecibirDano(DanoPorSegundo);
	}
}

