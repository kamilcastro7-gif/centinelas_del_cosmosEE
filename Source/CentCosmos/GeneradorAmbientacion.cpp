// Fill out your copyright notice in the Description page of Project Settings.

#include "GeneradorAmbientacion.h"
#include "ObstaculoNave.h"
#include "ObstaculoSatelite.h"
#include "ObstaculoRestos.h"
#include "Engine/World.h"

AGeneradorAmbientacion::AGeneradorAmbientacion()
{
	PrimaryActorTick.bCanEverTick = false;

	// COPIADO DE TU GUÍA: Asignamos los StaticClass en el constructor
	ClaseNave = AObstaculoNave::StaticClass();
	ClaseSatelite = AObstaculoSatelite::StaticClass();
	ClaseRestos = AObstaculoRestos::StaticClass();
}

void AGeneradorAmbientacion::BeginPlay()
{
	Super::BeginPlay();

	UWorld* Mundo = GetWorld();
	if (!Mundo) return;

	// Guía del bucle: Generamos 10 de cada uno (30 en total)
	for (int32 i = 0; i < 10; i++)
	{
		// Posiciones aleatorias en el rango de tu arena gris
		FVector PosNave = FVector(FMath::RandRange(-600, 600), FMath::RandRange(-600, 600), 150.0f);
		FVector PosSatelite = FVector(FMath::RandRange(-600, 600), FMath::RandRange(-600, 600), 150.0f);
		FVector PosRestos = FVector(FMath::RandRange(-600, 600), FMath::RandRange(-600, 600), 150.0f);

		FabricarObstaculoNave(PosNave, FRotator::ZeroRotator);
		FabricarObstaculoSatelite(PosSatelite, FRotator::ZeroRotator);
		FabricarObstaculoRestos(PosRestos, FRotator::ZeroRotator);
	}
}

// Métodos de fabricación que calcan el comportamiento del switch de tu amigo
AActor* AGeneradorAmbientacion::FabricarObstaculoNave(const FVector& Posicion, const FRotator& Rotacion)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return nullptr;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AObstaculoNave* Obstaculo = Mundo->SpawnActor<AObstaculoNave>(ClaseNave, Posicion, Rotacion, Params);
	if (Obstaculo)
	{
		Obstaculo->InicializarObstaculo();
		return Obstaculo;
	}
	return nullptr;
}

AActor* AGeneradorAmbientacion::FabricarObstaculoSatelite(const FVector& Posicion, const FRotator& Rotacion)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return nullptr;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AObstaculoSatelite* Obstaculo = Mundo->SpawnActor<AObstaculoSatelite>(ClaseSatelite, Posicion, Rotacion, Params);
	if (Obstaculo)
	{
		Obstaculo->InicializarObstaculo();
		return Obstaculo;
	}
	return nullptr;
}

AActor* AGeneradorAmbientacion::FabricarObstaculoRestos(const FVector& Posicion, const FRotator& Rotacion)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return nullptr;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AObstaculoRestos* Obstaculo = Mundo->SpawnActor<AObstaculoRestos>(ClaseRestos, Posicion, Rotacion, Params);
	if (Obstaculo)
	{
		Obstaculo->InicializarObstaculo();
		return Obstaculo;
	}
	return nullptr;
}