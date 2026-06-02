// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbstractFactory.generated.h"

UINTERFACE(MinimalAPI)
class UAbstractFactory : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interfaz Abstract Factory unificada.
 * Declara una familia de productos: obstáculos del entorno + enemigos.
 * Cada fábrica concreta implementa TODOS los métodos y decide
 * qué variante concreta spawnea — sin switch, sin lógica de selección aquí.
 */
class CENTCOSMOS_API IAbstractFactory
{
	GENERATED_BODY()

public:
	// ---  Obstáculos de ambientación ---
	virtual AActor* FabricarObstaculoNave(const FVector& Posicion, const FRotator& Rotacion) = 0;
	virtual AActor* FabricarObstaculoSatelite(const FVector& Posicion, const FRotator& Rotacion) = 0;
	virtual AActor* FabricarObstaculoRestos(const FVector& Posicion, const FRotator& Rotacion) = 0;

	// ---  Enemigos ---
	virtual AActor* FabricarVastago(const FVector& Posicion, const FRotator& Rotacion) = 0;
	virtual AActor* FabricarVigia(const FVector& Posicion, const FRotator& Rotacion) = 0;
	virtual AActor* FabricarHeraldo(const FVector& Posicion, const FRotator& Rotacion) = 0;
	virtual AActor* FabricarTribunalIra(const FVector& Posicion, const FRotator& Rotacion) = 0;
	virtual AActor* FabricarTribunalCaos(const FVector& Posicion, const FRotator& Rotacion) = 0;
	virtual AActor* FabricarTribAsedio(const FVector& Posicion, const FRotator& Rotacion) = 0;
	virtual AActor* FabricarTribunalSupremo(const FVector& Posicion, const FRotator& Rotacion) = 0;
	virtual AActor* FabricarTribunalTormenta(const FVector& Posicion, const FRotator& Rotacion) = 0;
	virtual AActor* FabricarTribunalVigilante(const FVector& Posicion, const FRotator& Rotacion) = 0;
};