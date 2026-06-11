// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "EnemDecorador.h"
#include "EnemVida.generated.h"

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
	virtual float GetVida() const override
	{
		return UEnemDecorador::GetVida() + VidaBonus;
	}
	virtual float GetVidaMaxima() const override
	{
		return UEnemDecorador::GetVidaMaxima() + VidaBonus;
	}

private:
	float VidaBonus = 0.f;
};