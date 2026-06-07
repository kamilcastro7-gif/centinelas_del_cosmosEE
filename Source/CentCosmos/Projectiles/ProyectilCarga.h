// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilCarga.generated.h"

class UNiagaraComponent; // NUEVO: Para el efecto Niagara

UCLASS()
class CENTCOSMOS_API AProyectilCarga : public AActor
{
    GENERATED_BODY()
public:
    AProyectilCarga();

    UPROPERTY()
    class UStaticMeshComponent* ProyectilMesh;

    // --- EFECTO VISUAL NIAGARA ---
    UPROPERTY(VisibleAnywhere, Category = "Componentes")
    UNiagaraComponent* EfectoNiagara;

    UPROPERTY()
    class UProjectileMovementComponent* ProjectileMovement;

    // ATRIBUTO DE DAÑO PÚBLICO
    UPROPERTY()
    float DanoBase;

    void InicializarCarga(float TiempoCarga);
    void LiberarProyectil(float TiempoCargaFinal, FVector DireccionLanzamiento);

    UFUNCTION()
    void AlChocar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
