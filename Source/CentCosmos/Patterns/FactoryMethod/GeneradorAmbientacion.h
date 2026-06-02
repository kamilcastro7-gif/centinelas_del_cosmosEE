// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbstractFactory.h"
#include "GeneradorAmbientacion.generated.h"

/**
 * Fábrica concreta A — especializada en obstáculos de ambientación.
 * Implementa toda la interfaz IAbstractFactory.
 * Los métodos de enemigos retornan nullptr: esta fábrica no los produce.
 */
UCLASS()
class CENTCOSMOS_API AGeneradorAmbientacion : public AActor, public IAbstractFactory
{
	GENERATED_BODY()

public:
	AGeneradorAmbientacion();

protected:
	virtual void BeginPlay() override;

public:
	// --- Familia A: implementación real ---
	virtual AActor* FabricarObstaculoNave(const FVector& Posicion, const FRotator& Rotacion) override;
	virtual AActor* FabricarObstaculoSatelite(const FVector& Posicion, const FRotator& Rotacion) override;
	virtual AActor* FabricarObstaculoRestos(const FVector& Posicion, const FRotator& Rotacion) override;

	// --- Familia B: no producida por esta fábrica ---
	virtual AActor* FabricarVastago(const FVector& Posicion, const FRotator& Rotacion) override { return nullptr; }
	virtual AActor* FabricarVigia(const FVector& Posicion, const FRotator& Rotacion) override { return nullptr; }
	virtual AActor* FabricarHeraldo(const FVector& Posicion, const FRotator& Rotacion) override { return nullptr; }
	virtual AActor* FabricarTribunalIra(const FVector& Posicion, const FRotator& Rotacion) override { return nullptr; }
	virtual AActor* FabricarTribunalCaos(const FVector& Posicion, const FRotator& Rotacion) override { return nullptr; }
	virtual AActor* FabricarTribAsedio(const FVector& Posicion, const FRotator& Rotacion) override { return nullptr; }
	virtual AActor* FabricarTribunalSupremo(const FVector& Posicion, const FRotator& Rotacion) override { return nullptr; }
	virtual AActor* FabricarTribunalTormenta(const FVector& Posicion, const FRotator& Rotacion) override { return nullptr; }
	virtual AActor* FabricarTribunalVigilante(const FVector& Posicion, const FRotator& Rotacion) override { return nullptr; }

private:
	TSubclassOf<class AObstaculoNave>     ClaseNave;
	TSubclassOf<class AObstaculoSatelite> ClaseSatelite;
	TSubclassOf<class AObstaculoRestos>   ClaseRestos;
};