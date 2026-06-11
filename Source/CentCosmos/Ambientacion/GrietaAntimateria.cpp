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

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Assetssss/A2/Meshy_AI_Celestial_Rift_in_Sto_0607091125_texture.Meshy_AI_Celestial_Rift_in_Sto_0607091125_texture'"));
    if (MeshAsset.Succeeded())
    {
        MeshGrieta->SetStaticMesh(MeshAsset.Object);

        // Mantienes tu escala original (Nota: mejor usar SetRelativeScale3D en el constructor)
        MeshGrieta->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));

        // --- AQUÍ ESTÁ LA MAGIA ---
        // Rotamos la malla 90 grados para que la cara frontal mire hacia la cámara
        // Si no queda exactamente de frente, cambia el 90.0f al segundo valor (Yaw) o al tercero (Roll)
        MeshGrieta->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    }

    MeshGrieta->SetCollisionProfileName(TEXT("BlockAll"));
    MeshGrieta->SetGenerateOverlapEvents(true);
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