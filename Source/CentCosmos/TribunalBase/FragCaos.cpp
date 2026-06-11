// Fill out your copyright notice in the Description page of Project Settings.

#include "FragCaos.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../Player/CentCosmosPawn.h"
// --- NUEVOS INCLUDES PARA NIAGARA ---
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AFragCaos::AFragCaos()
{
    PrimaryActorTick.bCanEverTick = true;

    // 1. HITBOX INVISIBLE
    MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaFrag"));
    RootComponent = MallaProyectil;

    MallaProyectil->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaProyectil->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    // --- LA SOLUCIÓN: FORZAR LOS EVENTOS DE IMPACTO ---
    MallaProyectil->SetGenerateOverlapEvents(true);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
    if (MeshAsset.Succeeded())
    {
        MallaProyectil->SetStaticMesh(MeshAsset.Object);
    }

    // Ocultamos visualmente
    MallaProyectil->SetHiddenInGame(true);

    // Opcional: Si notas que los fragmentos son muy difíciles de esquivar 
    // o que no te dan cuando deberían, puedes subirle la escala a 1.0f
    SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));

    // 2. EFECTO VISUAL NIAGARA
    EfectoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoNiagara"));
    EfectoNiagara->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Hit_3.NS_Hit_3'"));
    if (NiagaraAsset.Succeeded())
    {
        EfectoNiagara->SetAsset(NiagaraAsset.Object);
    }

    Velocidad = 1100.f;
}

void AFragCaos::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(2.0f);
}

void AFragCaos::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector DireccionHaciaAdelante = GetActorForwardVector();
    DireccionHaciaAdelante.Z = 0.f;
    DireccionHaciaAdelante.Normalize();

    FVector NuevoDesplazamiento = DireccionHaciaAdelante * Velocidad * DeltaTime;
    AddActorWorldOffset(NuevoDesplazamiento, true);
}

void AFragCaos::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
    {
        Cast<ACentCosmosPawn>(OtherActor)->RecibirDanioNave(10.0f);
        Destroy();
    }
}

