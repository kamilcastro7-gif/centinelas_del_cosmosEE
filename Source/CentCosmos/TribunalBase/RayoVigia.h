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
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UStaticMeshComponent* MallaRayo;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    class UNiagaraComponent* EfectoRayoNiagara;

    void ActualizarLongitudRayo();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion")
    float LongitudMaxima;

    // Aumentamos muchísimo la distancia para que no se apague al alejarte un poco
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion")
    float DistanciaActivacion = 15000.0f;

private:
    bool bPuedeHacerDanio;
    FTimerHandle TimerCooldownRayo;
    void ResetearDanioRayo();
};
