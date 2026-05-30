// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilPlasma.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AProyectilPlasma::AProyectilPlasma()
{
    PrimaryActorTick.bCanEverTick = true;

    MallaPlasma = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPlasma"));
    RootComponent = MallaPlasma;
    MallaPlasma->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaPlasma->SetMobility(EComponentMobility::Movable);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Pipe_90.Shape_Pipe_90'"));
    if (MeshAsset.Succeeded()) MallaPlasma->SetStaticMesh(MeshAsset.Object);

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

