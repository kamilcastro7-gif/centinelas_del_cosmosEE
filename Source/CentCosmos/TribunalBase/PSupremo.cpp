// Fill out your copyright notice in the Description page of Project Settings.

#include "PSupremo.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

APSupremo::APSupremo()
{
    PrimaryActorTick.bCanEverTick = true;

    MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPSupremo"));
    RootComponent = MallaProyectil;

    MallaProyectil->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaProyectil->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Trim_90_Out.Shape_Trim_90_Out'"));
    if (MeshAsset.Succeeded())
    {
        MallaProyectil->SetStaticMesh(MeshAsset.Object);
    }

    SetActorScale3D(FVector(0.7f, 0.7f, 0.7f));
    Velocidad = 1000.f;
}

void APSupremo::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(4.0f);
}

void APSupremo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector DireccionHaciaAdelante = GetActorForwardVector();
    DireccionHaciaAdelante.Z = 0.f;
    DireccionHaciaAdelante.Normalize();

    FVector NuevoDesplazamiento = DireccionHaciaAdelante * Velocidad * DeltaTime;
    AddActorWorldOffset(NuevoDesplazamiento, true);
}

