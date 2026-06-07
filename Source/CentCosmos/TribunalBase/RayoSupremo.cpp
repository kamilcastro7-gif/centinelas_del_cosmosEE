// Fill out your copyright notice in the Description page of Project Settings.

#include "RayoSupremo.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/CentCosmosPawn.h"
#include "NiagaraComponent.h" // NUEVO: Librería de Niagara

ARayoSupremo::ARayoSupremo()
{
    PrimaryActorTick.bCanEverTick = true;

    RaizGrupo = CreateDefaultSubobject<USceneComponent>(TEXT("RaizGrupo"));
    RootComponent = RaizGrupo;

    MallaAdvertenciaComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaAdvertenciaComp"));
    MallaAdvertenciaComp->SetupAttachment(RaizGrupo);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAdvAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    if (MeshAdvAsset.Succeeded()) MallaAdvertenciaComp->SetStaticMesh(MeshAdvAsset.Object);

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatAdvAsset(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse'"));
    if (MatAdvAsset.Succeeded()) MallaAdvertenciaComp->SetMaterial(0, MatAdvAsset.Object);

    // Corregimos la ubicación exagerada a solo 150 unidades hacia adelante
    MallaAdvertenciaComp->SetRelativeScale3D(FVector(35.0f, 0.05f, 0.2f));
    MallaAdvertenciaComp->SetRelativeLocation(FVector(150.f, 0.f, 0.f));
    MallaAdvertenciaComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


    MallaLetalComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaLetalComp"));
    MallaLetalComp->SetupAttachment(RaizGrupo);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshLetalAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
    if (MeshLetalAsset.Succeeded()) MallaLetalComp->SetStaticMesh(MeshLetalAsset.Object);

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatLetalAsset(TEXT("Material'/Game/StarterContent/Materials/M_Lava_With_Blends.M_Lava_With_Blends'"));
    if (MatLetalAsset.Succeeded()) MallaLetalComp->SetMaterial(0, MatLetalAsset.Object);

    MallaLetalComp->SetRelativeScale3D(FVector(35.0f, 2.5f, 3.0f));
    MallaLetalComp->SetRelativeLocation(FVector(150.f, 0.f, 0.f));
    MallaLetalComp->SetVisibility(false);
    MallaLetalComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MallaLetalComp->SetGenerateOverlapEvents(true);
    MallaLetalComp->SetHiddenInGame(true);

    EfectoRayoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoRayoNiagara"));
    EfectoRayoNiagara->SetupAttachment(RaizGrupo);

    // CORRECCIÓN: Lo ponemos en la misma ubicación que la malla (cerca)
    EfectoRayoNiagara->SetRelativeLocation(FVector(150.f, 0.f, 0.f));

    // CORRECCIÓN: Escala más pequeña en X para no deformar brutalmente la textura
    EfectoRayoNiagara->SetRelativeScale3D(FVector(8.0f, 1.5f, 1.5f));

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> RayoAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Ray_5.NS_Ray_5'"));
    if (RayoAsset.Succeeded())
    {
        EfectoRayoNiagara->SetAsset(RayoAsset.Object);
    }

    EfectoRayoNiagara->SetAutoActivate(false);
    bEsRayoLetal = false;
}

void ARayoSupremo::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(TimerFaseLetal, this, &ARayoSupremo::ActivarRayoLetal, 2.0f, false);
    SetLifeSpan(4.0f);
}

void ARayoSupremo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bEsRayoLetal)
    {
        TArray<AActor*> ActoresSuperpuestos;
        MallaLetalComp->GetOverlappingActors(ActoresSuperpuestos);

        for (AActor* Actor : ActoresSuperpuestos)
        {
            if (Actor && Actor->IsValidLowLevel() && Actor != this && Actor->IsA(ACentCosmosPawn::StaticClass()))
            {
                // INSTA-KILL ABSOLUTO
                Cast<ACentCosmosPawn>(Actor)->RecibirDanioNave(9999.0f);
            }
        }
    }
}

void ARayoSupremo::ActivarRayoLetal()
{
    bEsRayoLetal = true;

    if (MallaAdvertenciaComp)
    {
        MallaAdvertenciaComp->SetVisibility(false);
    }

    if (MallaLetalComp)
    {
        // Activa solo las colisiones, la malla sigue invisible gracias al HiddenInGame(true)
        MallaLetalComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
        MallaLetalComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }

    // --- NUEVO: Disparamos el efecto visual ---
    if (EfectoRayoNiagara)
    {
        EfectoRayoNiagara->Activate();
    }
}

