// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ambientacion.h"
#include "ObstaculoSatelite.generated.h"

UCLASS()
class CENTCOSMOS_API AObstaculoSatelite : public AAmbientacion
{
	GENERATED_BODY()

public:
	AObstaculoSatelite();
	virtual void InicializarObstaculo() override;
};
