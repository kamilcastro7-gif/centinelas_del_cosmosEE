// Fill out your copyright notice in the Description page of Project Settings.

#include "RayoSupremo.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

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

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatAdvAsset(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse'"));
    if (MatAdvAsset.Succeeded())
    {
        MallaAdvertenciaComp->SetMaterial(0, MatAdvAsset.Object);
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

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatLetalAsset(TEXT("Material'/Game/StarterContent/Materials/M_Lava_With_Blends.M_Lava_With_Blends'"));
    if (MatLetalAsset.Succeeded())
    {
        MallaLetalComp->SetMaterial(0, MatLetalAsset.Object);
    }

    MallaLetalComp->SetRelativeScale3D(FVector(35.0f, 2.5f, 3.0f));
    MallaLetalComp->SetRelativeLocation(FVector(1750.f, -100.f, -100.f));
    MallaLetalComp->SetVisibility(false);
    MallaLetalComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
            if (Actor && Actor->IsValidLowLevel() && Actor != this && Actor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
            {
                UGameplayStatics::ApplyDamage(Actor, 160.f * DeltaTime, nullptr, this, UDamageType::StaticClass());
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

