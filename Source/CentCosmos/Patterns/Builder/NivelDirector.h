// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INivelBuilder.h"
#include "NivelDirector.generated.h"

UCLASS()
class CENTCOSMOS_API ANivelDirector : public AActor
{
	GENERATED_BODY()

public:
	ANivelDirector();

	void SetBuilder(TScriptInterface<INivelBuilder> NuevoBuilder);

	// Construye nivel completo: ambientacion + enemigos + metadata
	FNivel ConstruirNivel(UWorld* World, const FString& Nombre, float TiempoLimite, float Dificultad);

	// Construye solo tutorial: ambientacion + pocos enemigos
	FNivel ConstruirTutorial(UWorld* World);

	// Construye una oleada sin ambientacion nueva
	FNivel ConstruirOleada(UWorld* World, float Dificultad);

private:
	TScriptInterface<INivelBuilder> Builder;
};