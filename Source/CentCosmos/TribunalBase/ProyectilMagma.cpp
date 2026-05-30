// Fill out your copyright notice in the Description page of Project Settings.


#include "ProyectilMagma.h"
#include "Tribunal_Ira.h"
#include "../EExclusivos/RastrosFuego.h" // Se mantiene intacto asumiendo que ya existe en tu proyecto
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"

AProyectilMagma::AProyectilMagma()
{
    PrimaryActorTick.bCanEverTick = true;

    MallaMagma = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaMagma"));
    RootComponent = MallaMagma;

    MallaMagma->SetMobility(EComponentMobility::Movable);

    MallaMagma->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    MallaMagma->SetGenerateOverlapEvents(true);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
    if (MeshAsset.Succeeded())
    {
        MallaMagma->SetStaticMesh(MeshAsset.Object);
    }

    SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));

    Velocidad = 1000.f;
    ContadorRebotes = 0;
    CronometroRastro = 0.f;
    IntervaloRastro = 0.12f;
    DireccionVuelo = FVector::ZeroVector;
    JefeEmisor = nullptr;
}

void AProyectilMagma::BeginPlay()
{
    Super::BeginPlay();

    if (JefeEmisor && MallaMagma)
    {
        MallaMagma->IgnoreActorWhenMoving(JefeEmisor, true);
    }
}

void AProyectilMagma::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (DireccionVuelo.IsZero()) return;

    CronometroRastro += DeltaTime;
    if (CronometroRastro >= IntervaloRastro)
    {
        SpawnRastroFuego();
        CronometroRastro = 0.f;
    }

    FVector PosicionActual = GetActorLocation();
    FVector Desplazamiento = DireccionVuelo * Velocidad * DeltaTime;
    FVector PosicionDestino = PosicionActual + Desplazamiento;

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    if (JefeEmisor)
    {
        QueryParams.AddIgnoredActor(JefeEmisor);
    }

    FCollisionShape CollisionShape;
    CollisionShape.SetSphere(100.f);

    bool bHuboChoque = GetWorld()->SweepSingleByProfile(
        HitResult,
        PosicionActual,
        PosicionDestino,
        FQuat::Identity,
        TEXT("BlockAllDynamic"),
        CollisionShape,
        QueryParams
    );

    if (bHuboChoque && HitResult.bBlockingHit)
    {
        AActor* Obstaculo = HitResult.GetActor();

        if (Obstaculo && (Obstaculo == JefeEmisor || Obstaculo->IsA(ARastrosFuego::StaticClass()) || Obstaculo->IsA(ATribunal_Ira::StaticClass())))
        {
            SetActorLocation(PosicionDestino);
            return;
        }

        ContadorRebotes++;

        if (ContadorRebotes >= 2)
        {
            if (JefeEmisor)
            {
                JefeEmisor->NotificarMuerteMagma();
            }
            Destroy();
            return;
        }

        FVector Normal = HitResult.ImpactNormal;
        Normal.Z = 0.f;
        Normal.Normalize();

        FVector DireccionReflejada = DireccionVuelo - 2.f * FVector::DotProduct(DireccionVuelo, Normal) * Normal;
        DireccionReflejada.Z = 0.f;
        DireccionReflejada.Normalize();

        DireccionVuelo = DireccionReflejada;

        SetActorRotation(DireccionVuelo.Rotation());

        SetActorLocation(HitResult.Location + (DireccionVuelo * 20.f));
    }
    else
    {
        SetActorLocation(PosicionDestino);
    }
}

void AProyectilMagma::SpawnRastroFuego()
{
    FVector PosicionSuelo = GetActorLocation();

    PosicionSuelo.Z = 180.f;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GetWorld()->SpawnActor<ARastrosFuego>(ARastrosFuego::StaticClass(), PosicionSuelo, FRotator::ZeroRotator, SpawnParams);
}

