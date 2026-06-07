// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PAntimateria.generated.h"

class UNiagaraComponent; // NUEVO: Para el sistema de partículas Niagara

UCLASS()
class CENTCOSMOS_API APAntimateria : public AActor
{
    GENERATED_BODY()

public:
    APAntimateria();

    // --- HITBOX INVISIBLE ---
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* ProyectilMesh;

    // --- EFECTO VISUAL NIAGARA ---
    UPROPERTY(VisibleAnywhere, Category = "Componentes")
    UNiagaraComponent* EfectoNiagara;

    // Exponemos esto para configurar la velocidad desde la Grieta
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    class UProjectileMovementComponent* ProjectileMovement;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};