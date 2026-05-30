// Fill out your copyright notice in the Description page of Project Settings.

#include "EExclusivo.h"
#include "Kismet/KismetMathLibrary.h"

AEExclusivo::AEExclusivo()
{
	PrimaryActorTick.bCanEverTick = true;

	// CREAMOS EL SUBOBJETO BASE AQUÍ
	EnemigoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemigoMesh"));
	RootComponent = EnemigoMesh;

	NivelExclusivo = 1;
	Velocidad = 250.0f;
	RangoMovimiento = 500.0f;
}

void AEExclusivo::BeginPlay()
{
	Super::BeginPlay();
	CalcularNuevaDireccion();
}

void AEExclusivo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector PosicionActual = GetActorLocation();
	FVector NuevaPosicion = FMath::VInterpTo(PosicionActual, DireccionObjetivo, DeltaTime, Velocidad / 100.f);

	NuevaPosicion.Z = PosicionActual.Z; // Restringido al plano horizontal
	SetActorLocation(NuevaPosicion);

	if (FVector::DistSquared(PosicionActual, DireccionObjetivo) < 900.0f)
	{
		CalcularNuevaDireccion();
	}
}

void AEExclusivo::CalcularNuevaDireccion()
{
	FVector Origen = GetActorLocation();
	float RandomX = UKismetMathLibrary::RandomFloatInRange(-RangoMovimiento, RangoMovimiento);
	float RandomY = UKismetMathLibrary::RandomFloatInRange(-RangoMovimiento, RangoMovimiento);

	DireccionObjetivo = FVector(Origen.X + RandomX, Origen.Y + RandomY, Origen.Z);
}
