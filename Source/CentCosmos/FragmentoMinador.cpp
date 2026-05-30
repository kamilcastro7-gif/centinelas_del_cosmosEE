// Fill out your copyright notice in the Description page of Project Settings.

#include "FragmentoMinador.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"

AFragmentoMinador::AFragmentoMinador()
{
	PrimaryActorTick.bCanEverTick = true;

	FragmentoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FragmentoMesh"));
	RootComponent = FragmentoMesh;

	// ¡MÁS VELOCIDAD! Subida de 600 a 1200 para que salgan disparados con furia
	VelocidadFragmento = 1200.0f;
	DireccionMovimiento = FVector::ZeroVector;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (MeshAsset.Succeeded())
	{
		FragmentoMesh->SetStaticMesh(MeshAsset.Object);
	}

	SetActorScale3D(FVector(0.4f, 0.4f, 0.4f));
	FragmentoMesh->SetCollisionProfileName(TEXT("BlockAll"));
}

void AFragmentoMinador::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(4.0f); // Se destruyen solos a los 4 segundos
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

