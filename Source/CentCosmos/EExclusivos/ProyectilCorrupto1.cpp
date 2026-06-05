// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilCorrupto1.h"
#include "../Player/CentCosmosPawn.h"

AProyectilCorrupto1::AProyectilCorrupto1()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TorusMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus'"));
	if (Asset.Succeeded()) Mesh->SetStaticMesh(Asset.Object);
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

void AProyectilCorrupto1::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
	{
		ACentCosmosPawn* Nave = Cast<ACentCosmosPawn>(OtherActor);
		if (Nave)
		{
			Nave->RecibirDanioNave(12.0f); // PROYECTIL 1: DAÑO 12
			Destroy();
		}
	}
}





