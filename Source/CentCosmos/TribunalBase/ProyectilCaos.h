// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilCaos.generated.h"

class UNiagaraComponent; // NUEVO: Para el sistema de partículas Niagara

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

    // --- HITBOX INVISIBLE ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    class UStaticMeshComponent* MallaProyectil;

    // --- EFECTO VISUAL NIAGARA ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UNiagaraComponent* EfectoNiagara;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float Velocidad;

    FVector DireccionVuelo;

private:
    FVector PosicionInicial;

    UPROPERTY(EditAnywhere, Category = "Configuracion Caos")
    float RangoMaximo;

    void DividirEnEstrella();
};
