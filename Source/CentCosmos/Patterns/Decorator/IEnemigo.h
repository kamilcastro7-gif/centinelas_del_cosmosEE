#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEnemigo.generated.h"

UINTERFACE(MinimalAPI)
class UEnemigo : public UInterface
{
	GENERATED_BODY()
};

class CENTCOSMOS_API IEnemigo : public IInterface
{
	GENERATED_BODY()

public:
	virtual float GetVida() const = 0;
	virtual float GetDanio() const = 0;
	virtual float GetVelocidad() const = 0;
	virtual void RecibirDanio(float Cantidad) = 0;
	virtual bool EstaVivo() const = 0;
};