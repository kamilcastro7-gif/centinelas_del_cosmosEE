// Fill out your copyright notice in the Description page of Project Settings.

#include "GrietaAntimateria.h"
#include "../Projectiles/CentCosmosProjectile.h"
#include "PAntimateria.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h" // �NECESARIO!

AGrietaAntimateria::AGrietaAntimateria()
{
    MeshGrieta = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshGrieta"));
    RootComponent = MeshGrieta;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
    if (MeshAsset.Succeeded())
    {
        MeshGrieta->SetStaticMesh(MeshAsset.Object);
        MeshGrieta->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));
    }

    MeshGrieta->SetCollisionProfileName(TEXT("BlockAll"));
    MeshGrieta->SetGenerateOverlapEvents(true);

    // ELIMIN� EL PROJECTILEMOVEMENT AQU� PORQUE LA GRIETA NO SE MUEVE
}

void AGrietaAntimateria::ProcesarImpacto()
{
    // El contador aumenta sin importar qu� tipo de proyectil sea
    ProyectilesRecibidos++;

    APawn* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Jugador)
    {
        FVector Dir = (Jugador->GetActorLocation() - GetActorLocation()).GetSafeNormal();

        for (int32 i = 0; i < ProyectilesRecibidos; i++)
        {
            APAntimateria* NuevaAntimateria = GetWorld()->SpawnActor<APAntimateria>(
                APAntimateria::StaticClass(),
                GetActorLocation() + (Dir * 100.0f),
                Dir.Rotation()
            );

            if (NuevaAntimateria && NuevaAntimateria->ProjectileMovement)
            {
                NuevaAntimateria->ProjectileMovement->Velocity = Dir * NuevaAntimateria->ProjectileMovement->InitialSpeed;
                NuevaAntimateria->ProjectileMovement->Activate(true);
            }
        }
        ProyectilesRecibidos = 0;
    }
}