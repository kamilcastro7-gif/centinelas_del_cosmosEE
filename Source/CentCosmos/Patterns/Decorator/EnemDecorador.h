// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IEnemigo.h"
#include "EnemDecorador.generated.h"

/**
 * Decorador abstracto — equivalente a Decorator en la plantilla.
 * Envuelve cualquier IEnemigo y delega todas las operaciones a él.
 * Los decoradores concretos heredan de esta clase y añaden comportamiento.
 */
UCLASS()
class CENTCOSMOS_API UEnemDecorador : public UObject, public IEnemigo
{
	GENERATED_BODY()

public:
	// Inicializa el decorador con el componente que va a envolver
	void Envolver(TScriptInterface<IEnemigo> Componente)
	{
		Inner = Componente;
	}

	// Delega todo al componente interior — igual que Decorator::Operation()
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
	// El componente envuelto — puede ser un base o OTRO decorador (cadena)
	UPROPERTY()
	TScriptInterface<IEnemigo> Inner;
};