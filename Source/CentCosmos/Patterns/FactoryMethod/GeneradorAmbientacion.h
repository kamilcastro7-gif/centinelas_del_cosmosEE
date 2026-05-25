// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbstractFactory.h"
#include "Ambientacion.h"
#include "GeneradorAmbientacion.generated.h"

UCLASS()
class CENTCOSMOS_API AGeneradorAmbientacion : public AActor, public IAbstractFactory
{
	GENERATED_BODY()

public:
	AGeneradorAmbientacion();

protected:
	virtual void BeginPlay() override;

public:
	// Implementación de la fábrica para tus 3 obstáculos
	virtual AActor* FabricarObstaculoNave(const FVector& Posicion, const FRotator& Rotacion) override;
	virtual AActor* FabricarObstaculoSatelite(const FVector& Posicion, const FRotator& Rotacion) override;
	virtual AActor* FabricarObstaculoRestos(const FVector& Posicion, const FRotator& Rotacion) override;

private:
	// Variables de clase idénticas a las de tu amigo
	TSubclassOf<class AObstaculoNave> ClaseNave;
	TSubclassOf<class AObstaculoSatelite> ClaseSatelite;
	TSubclassOf<class AObstaculoRestos> ClaseRestos;
};
