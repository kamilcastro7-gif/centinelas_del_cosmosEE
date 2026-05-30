// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NubeIonizada.generated.h"

UCLASS()
class CENTCOSMOS_API ANubeIonizada : public AActor
{
    GENERATED_BODY()

public:
    ANubeIonizada();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, Category = "Nebulosa")
    UStaticMeshComponent* MallaNube;

    UPROPERTY(EditAnywhere, Category = "Nebulosa")
    float VelocidadY;

    UPROPERTY(EditAnywhere, Category = "Nebulosa")
    float DuracionEfecto;

    UPROPERTY(EditAnywhere, Category = "Nebulosa")
    float RangoMovimientoY;

    // Esta es la función clave para detectar el choque
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
    float PosicionInicialY;
    float DireccionMovimiento;
};
