// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interfaz.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UAInterfaz : public UInterface
{
	GENERATED_BODY()
};

class CENTCOSMOS_API IAInterfaz
{
	GENERATED_BODY()

public:
	// Método polimórfico puro para los obstáculos
	virtual void InicializarObstaculo() = 0;
};
