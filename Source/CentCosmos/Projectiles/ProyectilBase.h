// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilBase.generated.h"

// NUEVO: Declaramos Niagara para poder usarlo
class UNiagaraComponent;

UCLASS()
class CENTCOSMOS_API AProyectilBase : public AActor
{
    GENERATED_BODY()
public:
    AProyectilBase();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    class UStaticMeshComponent* MallaProyectil;

    // NUEVO: Componente para el efecto de energía visual
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UNiagaraComponent* EfectoProyectil;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movimiento")
    class UProjectileMovementComponent* MovimientoProyectil;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Estadisticas")
    int32 VidaProyectil;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Estadisticas")
    bool bEsSeguidor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Estadisticas")
    float Danio;

    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};