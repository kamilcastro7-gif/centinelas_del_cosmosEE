// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"

AProyectilBase::AProyectilBase()
{
    PrimaryActorTick.bCanEverTick = true;

    MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaProyectil"));
    RootComponent = MallaProyectil;

    // REFERENCIA: Basic Shape Sphere del Engine
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
    if (MeshAsset.Succeeded()) {
        MallaProyectil->SetStaticMesh(MeshAsset.Object);
    }

    MovimientoProyectil = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovimientoProyectil"));
    MovimientoProyectil->bRotationFollowsVelocity = true;
    MovimientoProyectil->ProjectileGravityScale = 0.0f;

    VidaProyectil = 1;
    bEsSeguidor = false;
    InitialLifeSpan = 5.0f;
}

void AProyectilBase::BeginPlay() { Super::BeginPlay(); }

void AProyectilBase::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    FVector NuevaUbicacion = GetActorLocation() + (GetActorForwardVector() * MovimientoProyectil->InitialSpeed * DeltaTime);
    // TODO: Conflicto con UProjectileMovementComponent activo.
    // Usar solo uno: o el componente o este movimiento manual.
    SetActorLocation(NuevaUbicacion);

    if (bEsSeguidor) {
        AActor* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (Jugador) {
            FRotator RotacionObjetivo = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Jugador->GetActorLocation());
            FRotator NuevaRotacion = FMath::RInterpTo(GetActorRotation(), RotacionObjetivo, DeltaTime, 2.0f);
            SetActorRotation(NuevaRotacion);
        }
    }
}

void AProyectilBase::RecibirImpacto() {
    VidaProyectil--;
    if (VidaProyectil <= 0) Destroy();
}

