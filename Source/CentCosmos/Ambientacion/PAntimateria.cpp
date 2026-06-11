// Fill out your copyright notice in the Description page of Project Settings.

#include "PAntimateria.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../Player/CentCosmosPawn.h"
// --- NUEVOS INCLUDES PARA NIAGARA ---
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

APAntimateria::APAntimateria()
{
    // 1. HITBOX INVISIBLE: La esfera base
    ProyectilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProyectilMesh"));
    RootComponent = ProyectilMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
    if (SphereAsset.Succeeded())
    {
        ProyectilMesh->SetStaticMesh(SphereAsset.Object);
    }

    // Ocultamos la malla visualmente
    ProyectilMesh->SetHiddenInGame(true);

    // 2. CONFIGURACIÓN CRÍTICA DE COLISIÓN
    ProyectilMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ProyectilMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    ProyectilMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    // Forzamos los eventos de impacto para asegurar que el daño funcione siendo invisible
    ProyectilMesh->SetGenerateOverlapEvents(true);

    // 3. EFECTO VISUAL NIAGARA
    EfectoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoNiagara"));
    EfectoNiagara->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Hit_3.NS_Hit_3'"));
    if (NiagaraAsset.Succeeded())
    {
        EfectoNiagara->SetAsset(NiagaraAsset.Object);
    }

    // 4. Componente de movimiento
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = RootComponent; // Es mejor enlazarlo directo a la raíz
    ProjectileMovement->InitialSpeed = 1500.f;
    ProjectileMovement->MaxSpeed = 1500.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale = 0.f;

    // 5. Auto-destrucción
    InitialLifeSpan = 3.0f;
}

void APAntimateria::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
    {
        // Le aplica los 3 de daño
        Cast<ACentCosmosPawn>(OtherActor)->RecibirDanioNave(3.0f);

        // El proyectil se destruye
        Destroy();
    }
}