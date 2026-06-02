// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbstractFactory.h"
#include "EnemyFactory.generated.h"

/**
 * Fábrica concreta B — especializada en enemigos.
 * Implementa toda la interfaz IAbstractFactory.
 * Los métodos de obstáculos retornan nullptr: esta fábrica no los produce.
 * Sin switch, sin ETipoEnemigo — cada tipo tiene su propio método.
 */
UCLASS()
class CENTCOSMOS_API AEnemyFactory : public AActor, public IAbstractFactory
{
	GENERATED_BODY()

public:
	AEnemyFactory();

public:
	// --- Familia A: no producida por esta fábrica ---
	virtual AActor* FabricarObstaculoNave(const FVector& Posicion, const FRotator& Rotacion) override { return nullptr; }
	virtual AActor* FabricarObstaculoSatelite(const FVector& Posicion, const FRotator& Rotacion) override { return nullptr; }
	virtual AActor* FabricarObstaculoRestos(const FVector& Posicion, const FRotator& Rotacion) override { return nullptr; }

	// --- Familia B: implementación real ---
	virtual AActor* FabricarVastago(const FVector& Posicion, const FRotator& Rotacion) override;
	virtual AActor* FabricarVigia(const FVector& Posicion, const FRotator& Rotacion) override;
	virtual AActor* FabricarHeraldo(const FVector& Posicion, const FRotator& Rotacion) override;
	virtual AActor* FabricarTribunalIra(const FVector& Posicion, const FRotator& Rotacion) override;
	virtual AActor* FabricarTribunalCaos(const FVector& Posicion, const FRotator& Rotacion) override;
	virtual AActor* FabricarTribAsedio(const FVector& Posicion, const FRotator& Rotacion) override;
	virtual AActor* FabricarTribunalSupremo(const FVector& Posicion, const FRotator& Rotacion) override;
	virtual AActor* FabricarTribunalTormenta(const FVector& Posicion, const FRotator& Rotacion) override;
	virtual AActor* FabricarTribunalVigilante(const FVector& Posicion, const FRotator& Rotacion) override;
private:
	TSubclassOf<class AEVastago_Del_infierno> ClaseVastago;
	TSubclassOf<class AEVigia_Del_Vacio>      ClaseVigia;
	TSubclassOf<class AEHeraldo_De_La_Ruina>  ClaseHeraldo;
	TSubclassOf<class ATribunal_Ira>           ClaseTribunalIra;
	TSubclassOf<class ATribunalCaos>           ClaseTribunalCaos;
	TSubclassOf<class ATribAsedio>             ClaseTribAsedio;
	TSubclassOf<class ATribunalSupremo>        ClaseTribSupremo;
	TSubclassOf<class ATribunalTormenta>       ClaseTribTormenta;
	TSubclassOf<class ATribunalVigilante>     ClaseTribVigilante;
};