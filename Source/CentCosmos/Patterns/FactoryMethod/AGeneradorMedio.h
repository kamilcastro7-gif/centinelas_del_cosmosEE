#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IAbstractFactory.h"
#include "AGeneradorMedio.generated.h"

UCLASS()
class CENTCOSMOS_API AGeneradorMedio : public AActor, public IAbstractFactory
{
    GENERATED_BODY()
public:
    AGeneradorMedio();
    virtual AActor* FabricarObstaculo(const FVector& Posicion, const FRotator& Rotacion) override;
    virtual AActor* FabricarEnemigo(const FVector& Posicion, const FRotator& Rotacion) override;
private:
    TSubclassOf<class AObstaculoSatelite>     ClaseObstaculo;
    TSubclassOf<class AEVigia_Del_Vacio>      ClaseEnemigo;
};
