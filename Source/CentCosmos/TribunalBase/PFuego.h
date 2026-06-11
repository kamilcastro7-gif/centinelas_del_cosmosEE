// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PFuego.generated.h"

class UNiagaraComponent; // NUEVO: Para el sistema de partículas

UCLASS()
class CENTCOSMOS_API APFuego : public AActor
{
    GENERATED_BODY()

public:
    APFuego();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // --- HITBOX INVISIBLE ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UStaticMeshComponent* MallaEsfera;

    // --- EFECTO VISUAL NIAGARA ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UNiagaraComponent* EfectoFuego;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float Velocidad;

    FVector DireccionVuelo;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
