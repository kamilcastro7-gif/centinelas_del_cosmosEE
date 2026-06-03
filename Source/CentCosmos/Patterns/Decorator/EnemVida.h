// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "EnemDecorador.h"
#include "EnemVida.generated.h"

/**
 * Decorador concreto A — equivalente a ConcreteDecoratorA en la plantilla.
 * Suma VidaBonus al GetVida() del componente envuelto.
 * Se puede encadenar: new EnemVida(new EnemVida(Base, 150), 100) = base+250
 */
UCLASS()
class CENTCOSMOS_API UEnemVida : public UEnemDecorador
{
	GENERATED_BODY()

public:
	void InicializarBonus(TScriptInterface<IEnemigo> Componente, float Bonus)
	{
		Envolver(Componente);
		VidaBonus = Bonus;
	}

	// Suma el bonus a la vida del componente interior
	virtual float GetVida() const override
	{
		return UEnemDecorador::GetVida() + VidaBonus;
	}

	// La vida maxima tambien refleja el bonus
	virtual float GetVidaMaxima() const override
	{
		return UEnemDecorador::GetVidaMaxima() + VidaBonus;
	}

private:
	float VidaBonus = 0.f;
};