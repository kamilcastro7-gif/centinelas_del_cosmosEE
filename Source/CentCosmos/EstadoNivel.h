// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EstadoNivel.generated.h"

UINTERFACE(MinimalAPI)
class UEstadoNivel : public UInterface
{
	GENERATED_BODY()
};

class CENTCOSMOS_API IEstadoNivel
{
	GENERATED_BODY()

public:
	// Contrato obligatorio para todos los niveles
	virtual void IniciarNivel(class AGestorNiveles* Gestor) = 0;
	virtual void EjecutarTick(float DeltaTime) = 0;
};
