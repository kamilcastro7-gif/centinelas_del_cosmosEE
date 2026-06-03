// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IEnemigo.h"
#include "EnemBaseComp.generated.h"

/**
 * Componente concreto base — equivalente a ConcreteComponent en la plantilla.
 * Guarda la vida real del enemigo y la opera directamente.
 * Los decoradores envuelven este objeto.
 */
UCLASS()
class CENTCOSMOS_API UEnemBaseComp : public UObject, public IEnemigo
{
	GENERATED_BODY()

public:
	void Inicializar(float VidaInicial)
	{
		VidaActual = VidaInicial;
		VidaMaxima_ = VidaInicial;
	}

	virtual float GetVida()       const override { return VidaActual; }
	virtual float GetVidaMaxima() const override { return VidaMaxima_; }

	virtual void RecibirDanio(float Cantidad) override
	{
		VidaActual = FMath::Max(VidaActual - Cantidad, 0.0f);
	}

	virtual void RegenerarVida(float Bonus) override
	{
		VidaActual = FMath::Min(VidaActual + Bonus, VidaMaxima_);
	}

	virtual bool EstaVivo() const override { return VidaActual > 0.0f; }

private:
	float VidaActual = 0.f;
	float VidaMaxima_ = 0.f;
};