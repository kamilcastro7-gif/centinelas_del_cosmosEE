// Fill out your copyright notice in the Description page of Project Settings.

#include "RayoSupremo.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/CentCosmosPawn.h"

ARayoSupremo::ARayoSupremo()
{
    PrimaryActorTick.bCanEverTick = true;

    RaizGrupo = CreateDefaultSubobject<USceneComponent>(TEXT("RaizGrupo"));
    RootComponent = RaizGrupo;

    MallaAdvertenciaComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaAdvertenciaComp"));
    MallaAdvertenciaComp->SetupAttachment(RaizGrupo);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAdvAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    if (MeshAdvAsset.Succeeded())
    {
        MallaAdvertenciaComp->SetStaticMesh(MeshAdvAsset.Object);
    }

    if (MaterialAdvertencia)
    {
        MallaAdvertenciaComp->SetMaterial(0, MaterialAdvertencia);
    }

    MallaAdvertenciaComp->SetRelativeScale3D(FVector(35.0f, 0.05f, 0.2f));
    MallaAdvertenciaComp->SetRelativeLocation(FVector(1750.f, 0.f, 0.f));
    MallaAdvertenciaComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


    MallaLetalComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaLetalComp"));
    MallaLetalComp->SetupAttachment(RaizGrupo);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshLetalAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
    if (MeshLetalAsset.Succeeded())
    {
        MallaLetalComp->SetStaticMesh(MeshLetalAsset.Object);
    }

    if (MaterialLetal)
    {
        MallaLetalComp->SetMaterial(0, MaterialLetal);
    }

    MallaLetalComp->SetRelativeScale3D(FVector(35.0f, 2.5f, 3.0f));
    MallaLetalComp->SetRelativeLocation(FVector(1750.f, -100.f, -100.f));
    MallaLetalComp->SetVisibility(false);
    MallaLetalComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    bEsRayoLetal = false;
    MallaLetalComp->SetGenerateOverlapEvents(true);
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
        MallaLetalComp->SetVisibility(true);
        MallaLetalComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
        MallaLetalComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
}

