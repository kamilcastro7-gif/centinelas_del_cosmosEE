// Fill out your copyright notice in the Description page of Project Settings.

#include "PAntimateria.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../Player/CentCosmosPawn.h"

APAntimateria::APAntimateria()
{
    // 1. Crear malla
    ProyectilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProyectilMesh"));
    RootComponent = ProyectilMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
    if (SphereAsset.Succeeded())
    {
        ProyectilMesh->SetStaticMesh(SphereAsset.Object);
    }

    // 2. CONFIGURACIÓN CRÍTICA DE COLISIÓN
    // Esto hace que el proyectil sea "fantasma" y atraviese balas, jugadores y otros proyectiles
    ProyectilMesh->SetCollisionProfileName(TEXT("NoCollision"));
    ProyectilMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ProyectilMesh->SetGenerateOverlapEvents(true);

    // 3. Componente de movimiento
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 1500.f;
    ProjectileMovement->MaxSpeed = 1500.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale = 0.f;

    // 4. Auto-destrucción
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