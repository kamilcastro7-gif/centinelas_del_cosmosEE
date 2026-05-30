// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilCaos.generated.h"

UCLASS()
class CENTCOSMOS_API AProyectilCaos : public AActor
{
    GENERATED_BODY()

public:
    AProyectilCaos();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    class UStaticMeshComponent* MallaProyectil;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float Velocidad;

    FVector DireccionVuelo;

private:
    FVector PosicionInicial;

    UPROPERTY(EditAnywhere, Category = "Configuracion Caos")
    float RangoMaximo;

    void DividirEnEstrella();
};
