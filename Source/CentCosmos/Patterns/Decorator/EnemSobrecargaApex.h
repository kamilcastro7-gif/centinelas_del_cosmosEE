// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "EnemDecorador.h"
#include "EnemSobrecargaApex.generated.h"

UCLASS()
class CENTCOSMOS_API UEnemSobrecargaApex : public UEnemDecorador
{
	GENERATED_BODY()

public:
	void Inicializar(TScriptInterface<IEnemigo> Componente, float Duracion)
	{
		Envolver(Componente);
		DuracionEfecto = Duracion;
	}

	// Al morir el enemigo el GameMode o el Builder llama AplicarEfecto
	void AplicarEfecto(class ACentCosmosPawn* Nave);

private:
	float DuracionEfecto = 10.f;
};