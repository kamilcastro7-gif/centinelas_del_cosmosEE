#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IAbstractFactory.h"
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
    // Nueva interfaz — solo estos 2 métodos
    virtual AActor* FabricarObstaculo(const FVector& Posicion, const FRotator& Rotacion) override;
    virtual AActor* FabricarEnemigo(const FVector& Posicion, const FRotator& Rotacion) override;

private:
    TSubclassOf<class AObstaculoNave>     ClaseNave;
    TSubclassOf<class AObstaculoSatelite> ClaseSatelite;
    TSubclassOf<class AObstaculoRestos>   ClaseRestos;
};