// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilPlasma.generated.h"

UCLASS()
class CENTCOSMOS_API AProyectilPlasma : public AActor
{
    GENERATED_BODY()

public:
    AProyectilPlasma();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UStaticMeshComponent* MallaPlasma;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float VelocidadProyectil;

    FVector DireccionVuelo;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
    FVector PosicionInicial;
    bool bHaParado;
    float RangoMaximo;
};
