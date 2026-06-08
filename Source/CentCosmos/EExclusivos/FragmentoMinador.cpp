// Fill out your copyright notice in the Description page of Project Settings.

#include "FragmentoMinador.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "../Player/CentCosmosPawn.h" // NECESARIO PARA HACER DAÑO A LA NAVE

AFragmentoMinador::AFragmentoMinador()
{
	PrimaryActorTick.bCanEverTick = true;

	FragmentoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FragmentoMesh"));
	RootComponent = FragmentoMesh;

	VelocidadFragmento = 1200.0f;
	DireccionMovimiento = FVector::ZeroVector;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Assetssss/A7/Meshy_AI_Asteroid_Miner_0607154422_texture.Meshy_AI_Asteroid_Miner_0607154422_texture'"));
	if (MeshAsset.Succeeded()) FragmentoMesh->SetStaticMesh(MeshAsset.Object);

	SetActorScale3D(FVector(0.9f, 0.9f, 0.9f));

	// Configuramos para que pueda hacer "Overlap" con la nave
	FragmentoMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FragmentoMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AFragmentoMinador::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(4.0f);
}

void AFragmentoMinador::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!DireccionMovimiento.IsZero())
	{
		FVector NuevaPosicion = GetActorLocation() + (DireccionMovimiento * VelocidadFragmento * DeltaTime);
		SetActorLocation(NuevaPosicion);
	}
}

// --- NUEVO: APLICAR DAÑO ---
void AFragmentoMinador::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
	{
		ACentCosmosPawn* Nave = Cast<ACentCosmosPawn>(OtherActor);
		if (Nave)
		{
			Nave->RecibirDanioNave(6.0f); // DAÑO SEGÚN LISTA
			Destroy(); // Se destruye al impactar
		}
	}
}

