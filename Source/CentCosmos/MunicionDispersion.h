// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "PowerUp.h"
#include "MunicionDispersion.generated.h"

class UParticleSystemComponent; // NUEVO

UCLASS()
class CENTCOSMOS_API AMunicionDispersion : public APowerUp
{
	GENERATED_BODY()
public:
	AMunicionDispersion();
protected:
	virtual void AplicarEfecto(class ACentCosmosPawn* Nave) override;

private:
	// --- EFECTO VISUAL AURA ---
	UPROPERTY(VisibleAnywhere, Category = "Componentes")
	UParticleSystemComponent* EfectoAura;
};