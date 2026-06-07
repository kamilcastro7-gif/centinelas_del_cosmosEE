// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilCaos.h"
#include "FragCaos.h" 
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
// --- NUEVOS INCLUDES PARA NIAGARA ---
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AProyectilCaos::AProyectilCaos()
{
    PrimaryActorTick.bCanEverTick = true;

    // 1. HITBOX INVISIBLE: La cápsula base
    MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaProyectil"));
    RootComponent = MallaProyectil;

    MallaProyectil->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaProyectil->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
    if (MeshAsset.Succeeded())
    {
        MallaProyectil->SetStaticMesh(MeshAsset.Object);
    }

    // Ocultamos la cápsula visualmente
    MallaProyectil->SetHiddenInGame(true);
    SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));

    // 2. EFECTO VISUAL NIAGARA
    EfectoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoNiagara"));
    EfectoNiagara->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Hit_3.NS_Hit_3'"));
    if (NiagaraAsset.Succeeded())
    {
        EfectoNiagara->SetAsset(NiagaraAsset.Object);
    }

    Velocidad = 800.f;
    DireccionVuelo = FVector::ZeroVector;

    RangoMaximo = 650.f;
}

void AProyectilCaos::BeginPlay()
{
    Super::BeginPlay();
    PosicionInicial = GetActorLocation();
}

void AProyectilCaos::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector DireccionHaciaAdelante = GetActorForwardVector();
    DireccionHaciaAdelante.Z = 0.f;
    DireccionHaciaAdelante.Normalize();

    FVector NuevoDesplazamiento = DireccionHaciaAdelante * Velocidad * DeltaTime;
    AddActorWorldOffset(NuevoDesplazamiento, true);

    float DistanciaRecorrida = FVector::Dist(PosicionInicial, GetActorLocation());
    if (DistanciaRecorrida >= RangoMaximo)
    {
        DividirEnEstrella();
        Destroy();
    }
}

void AProyectilCaos::DividirEnEstrella()
{
    UWorld* World = GetWorld();
    if (!World) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FVector UbicacionDetonacion = GetActorLocation();

    for (int32 i = 0; i < 8; ++i)
    {
        float AnguloGrados = i * 45.0f;
        FRotator RotacionFragmento = FRotator(0.f, AnguloGrados, 0.f);

        World->SpawnActor<AFragCaos>(AFragCaos::StaticClass(), UbicacionDetonacion, RotacionFragmento, SpawnParams);
    }
}

