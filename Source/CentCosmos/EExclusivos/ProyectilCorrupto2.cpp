// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilCorrupto2.h"
#include "../Player/CentCosmosPawn.h"
// --- NUEVOS INCLUDES ---
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AProyectilCorrupto2::AProyectilCorrupto2()
{
    // 1. HITBOX INVISIBLE
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
    if (Asset.Succeeded()) Mesh->SetStaticMesh(Asset.Object);

    RootComponent = Mesh;
    Mesh->SetRelativeScale3D(FVector(2.5f));

    // Ocultamos la malla visualmente
    Mesh->SetHiddenInGame(true);

    // Colisiones
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    // 2. EFECTO VISUAL NIAGARA
    EfectoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoNiagara"));
    EfectoNiagara->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Hit_4.NS_Hit_4'"));
    if (NiagaraAsset.Succeeded())
    {
        EfectoNiagara->SetAsset(NiagaraAsset.Object);
    }

    Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    Movement->SetUpdatedComponent(RootComponent);
    Movement->bRotationFollowsVelocity = true;
    Movement->ProjectileGravityScale = 0.0f;
    InitialLifeSpan = 5.0f;
}

void AProyectilCorrupto2::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
    {
        ACentCosmosPawn* Nave = Cast<ACentCosmosPawn>(OtherActor);
        if (Nave)
        {
            Nave->RecibirDanioNave(25.0f); // PROYECTIL 2: DAÑO MASIVO 25
            Destroy();
        }
    }
}
