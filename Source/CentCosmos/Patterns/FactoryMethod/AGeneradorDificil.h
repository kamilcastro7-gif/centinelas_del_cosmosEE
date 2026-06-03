#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IAbstractFactory.h"
#include "AGeneradorDificil.generated.h"

UCLASS()
class CENTCOSMOS_API AGeneradorDificil : public AActor, public IAbstractFactory
{
    GENERATED_BODY()
public:
    AGeneradorDificil();
    virtual AActor* FabricarObstaculo(const FVector& Posicion, const FRotator& Rotacion) override;
    virtual AActor* FabricarEnemigo(const FVector& Posicion, const FRotator& Rotacion) override;
private:
    TSubclassOf<class AObstaculoRestos>       ClaseObstaculo;
    TSubclassOf<class AEHeraldo_De_La_Ruina>  ClaseEnemigo;
};