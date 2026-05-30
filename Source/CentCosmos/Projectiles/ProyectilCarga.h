// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilCarga.generated.h"

UCLASS()
class CENTCOSMOS_API AProyectilCarga : public AActor
{
    GENERATED_BODY()
public:
    AProyectilCarga();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    class UStaticMeshComponent* ProyectilMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    class UProjectileMovementComponent* ProjectileMovement;

    void InicializarCarga(float TiempoCarga);
    void LiberarProyectil(float TiempoCargaFinal, FVector DireccionLanzamiento);

    UFUNCTION()
    void AlChocar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
    float DanoBase;
};
