// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyFactory.generated.h"

UCLASS()
class CENTCOSMOS_API AEnemyFactory : public AActor
{
    GENERATED_BODY()
public:
    AEnemyFactory();

    // --- Enemigos básicos ---
    AActor* FabricarVastago(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarVigia(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarHeraldo(const FVector& Posicion, const FRotator& Rotacion);

    // --- Tribunales ---
    AActor* FabricarTribunalIra(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarTribunalCaos(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarTribAsedio(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarTribunalSupremo(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarTribunalTormenta(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarTribunalVigilante(const FVector& Posicion, const FRotator& Rotacion);

    // --- Enemigos exclusivos ---
    AActor* FabricarDronAnclaje(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarMinadorRocoso(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarBoyaElectrostatica(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarBombarderoFuego(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarEspectroErrante(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarEcoCorrupto(const FVector& Posicion, const FRotator& Rotacion);

private:
    TSubclassOf<class AEVastago_Del_infierno> ClaseVastago;
    TSubclassOf<class AEVigia_Del_Vacio>      ClaseVigia;
    TSubclassOf<class AEHeraldo_De_La_Ruina>  ClaseHeraldo;
    TSubclassOf<class ATribunal_Ira>           ClaseTribunalIra;
    TSubclassOf<class ATribunalCaos>           ClaseTribunalCaos;
    TSubclassOf<class ATribAsedio>             ClaseTribAsedio;
    TSubclassOf<class ATribunalSupremo>        ClaseTribSupremo;
    TSubclassOf<class ATribunalTormenta>       ClaseTribTormenta;
    TSubclassOf<class ATribunalVigilante>      ClaseTribVigilante;
    TSubclassOf<class ADronAnclaje>            ClaseDronAnclaje;
    TSubclassOf<class AMinadorRocoso>          ClaseMinadorRocoso;
    TSubclassOf<class ABoyaElectrostatica>     ClaseBoyaElectrostatica;
    TSubclassOf<class ABombarderoFuego>        ClaseBombarderoFuego;
    TSubclassOf<class AEspectroErrante>        ClaseEspectroErrante;
    TSubclassOf<class AEcoCorrupto>            ClaseEcoCorrupto;
};
