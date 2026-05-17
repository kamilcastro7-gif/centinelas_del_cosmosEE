// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enjambre.h"
#include "EVastago_Del_infierno.generated.h"

UCLASS()
class CENTCOSMOS_API AEVastago_Del_infierno : public AEnjambre
{
    GENERATED_BODY()
public:
    AEVastago_Del_infierno();
    void moverVastago();
    void VasAtacar();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Combate")
    float VelocidadVastago;

private:
    float TimerEstado;
    bool bEstaRodeando;
    FVector DireccionDispersion;
};
