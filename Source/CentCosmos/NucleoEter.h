// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUp.h"
#include "NucleoEter.generated.h"

UCLASS()
class CENTCOSMOS_API ANucleoEter : public APowerUp
{
    GENERATED_BODY()

public:
    ANucleoEter();

protected:
    virtual void AplicarEfecto(class ACentCosmosPawn* Nave) override;

private:
    float CantidadCuracion;
};
