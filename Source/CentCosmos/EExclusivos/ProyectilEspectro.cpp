// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilEspectro.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"

AProyectilEspectro::AProyectilEspectro()
{
	PrimaryActorTick.bCanEverTick = true;

	ProyectilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProyectilMesh"));
	RootComponent = ProyectilMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SphereMesh.Succeeded())
	{
		ProyectilMesh->SetStaticMesh(SphereMesh.Object);
	}

	ProyectilMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProyectilMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	SetActorScale3D(FVector(0.4f, 0.4f, 0.4f));
	// Cambia esta línea dentro del constructor AProyectilEspectro::AProyectilEspectro()
	Velocidad = 2300.0f; // Ataque ráfaga veloz para poner a prueba los reflejos del jugador
}

void AProyectilEspectro::BeginPlay()
{
	AActor::BeginPlay();
	SetLifeSpan(3.0f); // Límite para que no viaje al infinito
}

void AProyectilEspectro::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
	FVector NuevaPosicion = GetActorLocation() + (GetActorForwardVector() * Velocidad * DeltaTime);
	SetActorLocation(NuevaPosicion);
}

