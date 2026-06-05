// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "RayoVigia.generated.h"

UCLASS()
class CENTCOSMOS_API ARayoVigia : public AActor
{
    GENERATED_BODY()

public:
    ARayoVigia();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UStaticMeshComponent* MallaRayo;

    void ActualizarLongitudRayo();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion")
    float LongitudMaxima;

private:
    bool bPuedeHacerDanio;
    FTimerHandle TimerCooldownRayo;
    void ResetearDanioRayo();
};
