// Fill out your copyright notice in the Description page of Project Settings.

#include "FragCaos.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../Player/CentCosmosPawn.h"

AFragCaos::AFragCaos()
{
    PrimaryActorTick.bCanEverTick = true;

    MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaFrag"));
    RootComponent = MallaProyectil;

    MallaProyectil->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaProyectil->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
    if (MeshAsset.Succeeded())
    {
        MallaProyectil->SetStaticMesh(MeshAsset.Object);
    }

    SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));

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

