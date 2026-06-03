// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "EnemDecorador.h"
#include "EnemDisparoTriple.generated.h"

/**
 * Decorador concreto C.
 * Aplica el efecto DisparoTriple a la nave del jugador al activarse.
 * Delega toda la gestion de vida al componente interior sin modificarla.
 */
UCLASS()
class CENTCOSMOS_API UEnemDisparoTriple : public UEnemDecorador
{
	GENERATED_BODY()

public:
	void Inicializar(TScriptInterface<IEnemigo> Componente, float Duracion)
	{
		Envolver(Componente);
		DuracionEfecto = Duracion;
	}

	void AplicarEfecto(class ACentCosmosPawn* Nave);

private:
	float DuracionEfecto = 10.f;
};