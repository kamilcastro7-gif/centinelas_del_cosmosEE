// Fill out your copyright notice in the Description page of Project Settings.

#include "Ambientacion.h"
#include "Components/StaticMeshComponent.h"

AAmbientacion::AAmbientacion()
{
	PrimaryActorTick.bCanEverTick = false;

	// Creamos el componente vacío. ¡YA NO BUSCAMOS NINGUNA RUTA AQUÍ!
	MallaObstaculo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaObstaculo"));
	if (MallaObstaculo)
	{
		RootComponent = MallaObstaculo;
	}
}

void AAmbientacion::BeginPlay()
{
	Super::BeginPlay();
}

void AAmbientacion::InicializarObstaculo()
{
	// Método base que será sobrescrito por las hijas
}

