#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IEnemigo.h"
#include "EnemDecorador.generated.h"

UCLASS(BlueprintType)
class CENTCOSMOS_API UEnemDecorador : public UObject, public IEnemigo
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TScriptInterface<IEnemigo> Inner;

public:
	void Initialize(UObject* InInner) { Inner.SetObject(InInner); }

	virtual float GetVida() const override
	{
		return Inner ? Inner->GetVida() : 0.0f;
	}

	virtual float GetDanio() const override
	{
		return Inner ? Inner->GetDanio() : 0.0f;
	}

	virtual float GetVelocidad() const override
	{
		return Inner ? Inner->GetVelocidad() : 0.0f;
	}

	virtual void RecibirDanio(float Cantidad) override
	{
		if (Inner) Inner->RecibirDanio(Cantidad);
	}

	virtual bool EstaVivo() const override
	{
		return Inner ? Inner->EstaVivo() : false;
	}
};