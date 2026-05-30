// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilFuego.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "RastrosFuego.h"
#include "Engine/World.h"

AProyectilFuego::AProyectilFuego()
{
	PrimaryActorTick.bCanEverTick = true;

	ProyectilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProyectilMesh"));
	RootComponent = ProyectilMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (CubeMesh.Succeeded())
	{
		ProyectilMesh->SetStaticMesh(CubeMesh.Object);
	}

	SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
	Velocidad = 1100.0f;

	// =========================================================================
	// CORRECCIÓN DE COLISIÓN: Evita atascarse físicamente y romper los Timers
	// =========================================================================
	ProyectilMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProyectilMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AProyectilFuego::BeginPlay()
{
	AActor::BeginPlay();

	// REFUERZO ANTI-INFINITO: Clavamos el tiempo de vida máximo (3 segundos)
	SetLifeSpan(3.0f);

	// CORRECCIÓN: Usamos el componente de malla (ChispaMesh/ProyectilMesh) para ignorar al dueño
	AActor* Dueno = GetOwner();
	if (Dueno && ProyectilMesh)
	{
		ProyectilMesh->IgnoreActorWhenMoving(Dueno, true);
	}

	// Sigue sembrando fuego en el camino cada 0.12 segundos
	GetWorld()->GetTimerManager().SetTimer(TimerRastrosHandle, this, &AProyectilFuego::SoltarRastro, 0.12f, true);
}

void AProyectilFuego::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	FVector NuevaPosicion = GetActorLocation() + (GetActorForwardVector() * Velocidad * DeltaTime);
	SetActorLocation(NuevaPosicion);
}

void AProyectilFuego::SoltarRastro()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector UbicacionRastro = GetActorLocation();
		FRotator RotacionRastro = FRotator::ZeroRotator;

		World->SpawnActor<ARastrosFuego>(ARastrosFuego::StaticClass(), UbicacionRastro, RotacionRastro, SpawnParams);
	}
}

