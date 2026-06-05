// Fill out your copyright notice in the Description page of Project Settings.

#include "PRafaga.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../Player/CentCosmosPawn.h"

APRafaga::APRafaga()
{
    PrimaryActorTick.bCanEverTick = true;

    MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPRafaga"));
    RootComponent = MallaProyectil;

    MallaProyectil->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaProyectil->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    // CORRECCIÓN APLICADA: Actualizado a la referencia correcta Shape_Trim
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Trim.Shape_Trim'"));
    if (MeshAsset.Succeeded())
    {
        MallaProyectil->SetStaticMesh(MeshAsset.Object);
    }

    SetActorScale3D(FVector(0.8f, 0.8f, 0.8f));
    Velocidad = 1200.f;
}

void APRafaga::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(4.0f);
}

void APRafaga::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector DireccionHaciaAdelante = GetActorForwardVector();
    DireccionHaciaAdelante.Z = 0.f;
    DireccionHaciaAdelante.Normalize();

    FVector NuevoDesplazamiento = DireccionHaciaAdelante * Velocidad * DeltaTime;
    AddActorWorldOffset(NuevoDesplazamiento, true);
}

void APRafaga::NotifyActorBeginOverlap(AActor* OtherActor) // Haz lo mismo para APSupremo
{
    Super::NotifyActorBeginOverlap(OtherActor);
    if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
    {
        Cast<ACentCosmosPawn>(OtherActor)->RecibirDanioNave(10.0f);
        Destroy();
    }
}
