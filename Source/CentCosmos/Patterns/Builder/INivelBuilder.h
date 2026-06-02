// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FNivel.h"
#include "INivelBuilder.generated.h"

UINTERFACE(MinimalAPI)
class UNivelBuilder : public UInterface
{
	GENERATED_BODY()
};

class CENTCOSMOS_API INivelBuilder
{
	GENERATED_BODY()

public:
	virtual void Reset() = 0;
	virtual void SetMetadatos(const FString& Nombre, float TiempoLimite) = 0;
	virtual void SetDificultad(float Dificultad) = 0;

	// Cada builder decide que enemigos y cuantos spawnea
	virtual void AgregarEnemigos(UWorld* World) = 0;

	// Cada builder genera la ambientacion del nivel
	virtual void AgregarAmbientacion(UWorld* World) = 0;

	virtual FNivel ObtenerNivel() = 0;
};