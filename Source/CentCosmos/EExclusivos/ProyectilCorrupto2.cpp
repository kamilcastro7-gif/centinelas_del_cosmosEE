// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilCorrupto2.h"
#include "../Player/CentCosmosPawn.h"

AProyectilCorrupto2::AProyectilCorrupto2()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (Asset.Succeeded()) Mesh->SetStaticMesh(Asset.Object);
	Mesh->SetRelativeScale3D(FVector(2.5f));
	RootComponent = Mesh;

	// --- CORRECCIÓN DE COLISIÓN ---
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore); // Ignora tus proyectiles y paredes
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Solo detecta a tu nave

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->SetUpdatedComponent(RootComponent);
	Movement->bRotationFollowsVelocity = true;
	Movement->ProjectileGravityScale = 0.0f;
	InitialLifeSpan = 5.0f;
}

void AProyectilCorrupto2::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
    {
        ACentCosmosPawn* Nave = Cast<ACentCosmosPawn>(OtherActor);
        if (Nave)
        {
            Nave->RecibirDanioNave(25.0f); // PROYECTIL 2: DAÑO MASIVO 25
            Destroy();
        }
    }
}
