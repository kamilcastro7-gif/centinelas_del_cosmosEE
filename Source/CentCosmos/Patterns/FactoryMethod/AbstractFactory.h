// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbstractFactory.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UAbstractFactory : public UInterface
{
	GENERATED_BODY()
};

class CENTCOSMOS_API IAbstractFactory
{
	GENERATED_BODY()

public:
	virtual AActor* FabricarObstaculoNave(const FVector& Posicion, const FRotator& Rotacion) = 0;
	virtual AActor* FabricarObstaculoSatelite(const FVector& Posicion, const FRotator& Rotacion) = 0;
	virtual AActor* FabricarObstaculoRestos(const FVector& Posicion, const FRotator& Rotacion) = 0;
};