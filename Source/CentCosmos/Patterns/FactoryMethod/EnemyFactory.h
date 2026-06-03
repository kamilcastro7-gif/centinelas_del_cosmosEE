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

    AActor* FabricarVastago(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarVigia(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarHeraldo(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarTribunalIra(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarTribunalCaos(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarTribAsedio(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarTribunalSupremo(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarTribunalTormenta(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarTribunalVigilante(const FVector& Posicion, const FRotator& Rotacion);

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
};