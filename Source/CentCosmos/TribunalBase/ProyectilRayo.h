// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilRayo.generated.h"

class UNiagaraComponent; // NUEVO: Sistema de partículas

UCLASS()
class CENTCOSMOS_API AProyectilRayo : public AActor
{
    GENERATED_BODY()

public:
    AProyectilRayo();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnComponenteOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // --- HITBOX INVISIBLE ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UStaticMeshComponent* MallaRayo;

    // --- EFECTO VISUAL NIAGARA ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UNiagaraComponent* EfectoNiagara;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float VelocidadProyectil;

    FVector DireccionVuelo;
};
