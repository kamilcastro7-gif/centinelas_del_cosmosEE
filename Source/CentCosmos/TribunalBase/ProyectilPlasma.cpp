// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilPlasma.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../Player/CentCosmosPawn.h"
// --- NUEVOS INCLUDES ---
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AProyectilPlasma::AProyectilPlasma()
{
    PrimaryActorTick.bCanEverTick = true;

    // 1. HITBOX INVISIBLE
    MallaPlasma = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPlasma"));
    RootComponent = MallaPlasma;

    MallaPlasma->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaPlasma->SetGenerateOverlapEvents(true);
    MallaPlasma->SetMobility(EComponentMobility::Movable);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Pipe_90.Shape_Pipe_90'"));
    if (MeshAsset.Succeeded()) MallaPlasma->SetStaticMesh(MeshAsset.Object);

    // Ocultamos la malla visualmente
    MallaPlasma->SetHiddenInGame(true);

    // 2. EFECTO VISUAL NIAGARA
    EfectoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoNiagara"));
    EfectoNiagara->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Hit_1.NS_Hit_1'"));
    if (NiagaraAsset.Succeeded())
    {
        EfectoNiagara->SetAsset(NiagaraAsset.Object);
    }

    VelocidadProyectil = 900.f;
    RangoMaximo = 850.f;
    bHaParado = false;
    DireccionVuelo = FVector::ZeroVector;
}

void AProyectilPlasma::BeginPlay()
{
    Super::BeginPlay();
    PosicionInicial = GetActorLocation();
}

void AProyectilPlasma::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bHaParado && !DireccionVuelo.IsZero())
    {
        FVector NuevaPosicion = GetActorLocation() + (DireccionVuelo * VelocidadProyectil * DeltaTime);
        SetActorLocation(NuevaPosicion, false);

        float DistanciaAct = FVector::Dist(PosicionInicial, GetActorLocation());
        if (DistanciaAct >= RangoMaximo)
        {
            bHaParado = true;
            SetLifeSpan(3.0f);
        }
    }
}

void AProyectilPlasma::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
    {
        float DanoInfligido = bHaParado ? 8.0f : 10.0f;
        Cast<ACentCosmosPawn>(OtherActor)->RecibirDanioNave(DanoInfligido);
        Destroy();
    }
}

