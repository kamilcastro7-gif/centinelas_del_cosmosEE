// Fill out your copyright notice in the Description page of Project Settings.

#include "HologramaCaos.h"
#include "ProyectilCaos.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AHologramaCaos::AHologramaCaos()
{
    PrimaryActorTick.bCanEverTick = false;

    MallaHolograma = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaHolograma"));
    RootComponent = MallaHolograma;

    MallaHolograma->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Assetssss/A17/Meshy_AI_Nebula_Leviathan_0607222721_texture.Meshy_AI_Nebula_Leviathan_0607222721_texture'"));
    if (MeshAsset.Succeeded())
    {
        MallaHolograma->SetStaticMesh(MeshAsset.Object);
    }

    SetActorScale3D(FVector(2.5f, 2.5f, 2.5f));
}

void AHologramaCaos::Disparar()
{
    APawn* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Jugador) return;

    FVector Direccion = (Jugador->GetActorLocation() - GetActorLocation());
    Direccion.Z = 0.f;
    Direccion.Normalize();

    FVector SpawnLoc = GetActorLocation() + (Direccion * 200.f);
    FRotator SpawnRot = Direccion.Rotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GetWorld()->SpawnActor<AProyectilCaos>(AProyectilCaos::StaticClass(), SpawnLoc, SpawnRot, SpawnParams);
}

