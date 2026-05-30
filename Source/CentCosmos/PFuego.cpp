// Fill out your copyright notice in the Description page of Project Settings.

#include "PFuego.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

APFuego::APFuego()
{
    PrimaryActorTick.bCanEverTick = true;

    MallaEsfera = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaEsfera"));
    RootComponent = MallaEsfera;
    MallaEsfera->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaEsfera->SetMobility(EComponentMobility::Movable);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
    if (MeshAsset.Succeeded())
    {
        MallaEsfera->SetStaticMesh(MeshAsset.Object);
    }

    SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));

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

