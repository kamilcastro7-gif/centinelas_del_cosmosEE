#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IEnemigo.h"
#include "EnemBasico.generated.h"

UCLASS()
class CENTCOSMOS_API AEnemBasico : public ACharacter, public IEnemigo
{
    GENERATED_BODY()

public:
    AEnemBasico();

    virtual float GetVida() const override { return Vida; }
    virtual float GetDanio() const override { return Danio; }
    virtual float GetVelocidad() const override { return Velocidad; }
	virtual void RecibirDanio(float Cantidad) override { Vida -= Cantidad; }
	virtual bool EstaVivo() const override { return Vida > 0.0f; }

	void SetVida(float NuevaVida) { Vida = NuevaVida; }
	void SetDanio(float NuevoDanio) { Danio = NuevoDanio; }
	void SetVelocidad(float NuevaVelocidad) { Velocidad = NuevaVelocidad; }

private:
	float Vida;
	float Danio;
	float Velocidad;
};