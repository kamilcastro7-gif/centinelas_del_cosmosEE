#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IObstaculo.h"
#include "IAbstractFactory.generated.h"

UINTERFACE(MinimalAPI)
class UAbstractFactory : public UInterface { GENERATED_BODY() };

class CENTCOSMOS_API IAbstractFactory
{
    GENERATED_BODY()
public:
    // Equivale a CreateProductA() del canónico
    virtual AActor* FabricarObstaculo(const FVector& Posicion, const FRotator& Rotacion) = 0;

    // Equivale a CreateProductB() del canónico
    virtual AActor* FabricarEnemigo(const FVector& Posicion, const FRotator& Rotacion) = 0;
};