// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TribunalBase.h"
#include "Components/ChildActorComponent.h"
#include "TribunalVigilante.generated.h"

UCLASS()
class CENTCOSMOS_API ATribunalVigilante : public ATribunalBase
{
    GENERATED_BODY()

public:
    ATribunalVigilante();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Fases")
    void ActivarJefe();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UChildActorComponent* ComponenteRayo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion")
    float VelocidadGiro;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion")
    float RangoDeteccion;
};
