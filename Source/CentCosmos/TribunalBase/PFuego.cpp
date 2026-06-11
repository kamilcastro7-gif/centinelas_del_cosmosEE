// Fill out your copyright notice in the Description page of Project Settings.

#include "PFuego.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../Player/CentCosmosPawn.h"
// --- NUEVOS INCLUDES PARA NIAGARA ---
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

APFuego::APFuego()
{
    PrimaryActorTick.bCanEverTick = true;

    // 1. HITBOX: Esfera invisible
    MallaEsfera = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaEsfera"));
    RootComponent = MallaEsfera;
    MallaEsfera->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaEsfera->SetMobility(EComponentMobility::Movable);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
    if (MeshAsset.Succeeded())
    {
        MallaEsfera->SetStaticMesh(MeshAsset.Object);
    }

    // Ocultamos la esfera visualmente
    MallaEsfera->SetHiddenInGame(true);
    SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));

    // 2. EFECTO VISUAL: Niagara (NS_Energy_5)
    EfectoFuego = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoFuego"));
    EfectoFuego->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Energy_5.NS_Energy_5'"));
    if (NiagaraAsset.Succeeded())
    {
        EfectoFuego->SetAsset(NiagaraAsset.Object);
    }

    Velocidad = 1200.f;
    DireccionVuelo = FVector::ZeroVector;
}

void APFuego::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(3.5f);
}

void APFuego::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!DireccionVuelo.IsZero())
    {
        FVector NuevaPosicion = GetActorLocation() + (DireccionVuelo * Velocidad * DeltaTime);
        SetActorLocation(NuevaPosicion, false);
    }
}

void APFuego::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
    {
        Cast<ACentCosmosPawn>(OtherActor)->RecibirDanioNave(10.0f);
        Destroy();
    }
}

