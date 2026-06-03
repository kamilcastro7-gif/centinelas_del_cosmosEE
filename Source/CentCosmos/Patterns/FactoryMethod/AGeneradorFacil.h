#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IAbstractFactory.h"
#include "AGeneradorFacil.generated.h"

UCLASS()
class CENTCOSMOS_API AGeneradorFacil : public AActor, public IAbstractFactory
{
    GENERATED_BODY()
public:
    AGeneradorFacil();

    // Obstáculo normal + Vástago (enemigo fácil)
    virtual AActor* FabricarObstaculo(const FVector& Posicion, const FRotator& Rotacion) override;
    virtual AActor* FabricarEnemigo(const FVector& Posicion, const FRotator& Rotacion) override;

private:
    TSubclassOf<class AObstaculoNave>            ClaseObstaculo;
    TSubclassOf<class AEVastago_Del_infierno>    ClaseEnemigo;
};