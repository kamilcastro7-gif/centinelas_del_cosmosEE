// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProyectilCorrupto2.generated.h"

class UNiagaraComponent; // NUEVO

UCLASS()
class CENTCOSMOS_API AProyectilCorrupto2 : public AActor
{
    GENERATED_BODY()
public:
    AProyectilCorrupto2();
    UPROPERTY(VisibleAnywhere) UStaticMeshComponent* Mesh;

    // --- EFECTO VISUAL NIAGARA ---
    UPROPERTY(VisibleAnywhere, Category = "Componentes")
    UNiagaraComponent* EfectoNiagara;

    UPROPERTY(VisibleAnywhere) UProjectileMovementComponent* Movement;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};