#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IObstaculo.generated.h"

UINTERFACE(MinimalAPI)
class UObstaculo : public UInterface { GENERATED_BODY() };

class CENTCOSMOS_API IObstaculo
{
    GENERATED_BODY()
public:
    virtual void InicializarObstaculo() = 0;
};