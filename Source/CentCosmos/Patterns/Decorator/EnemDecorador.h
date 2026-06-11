// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IEnemigo.h"
#include "EnemDecorador.generated.h"

UCLASS()
class CENTCOSMOS_API UEnemDecorador : public UObject, public IEnemigo
{
	GENERATED_BODY()

public:
	void Envolver(TScriptInterface<IEnemigo> Componente)
	{
		Inner = Componente;
	}

	virtual float GetVida()       const override
	{
		return Inner ? Inner->GetVida() : 0.f;
	}

	virtual float GetVidaMaxima() const override
	{
		return Inner ? Inner->GetVidaMaxima() : 0.f;
	}

	virtual void RecibirDanio(float Cantidad) override
	{
		if (Inner) Inner->RecibirDanio(Cantidad);
	}

	virtual void RegenerarVida(float Bonus) override
	{
		if (Inner) Inner->RegenerarVida(Bonus);
	}

	virtual bool EstaVivo() const override
	{
		return Inner ? Inner->EstaVivo() : false;
	}

protected:
	UPROPERTY()
	TScriptInterface<IEnemigo> Inner;
};