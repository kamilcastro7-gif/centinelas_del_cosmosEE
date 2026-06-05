// Fill out your copyright notice in the Description page of Project Settings.

#include "Escudo.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"

AEscudo::AEscudo()
{
	PrimaryActorTick.bCanEverTick = false;

	// 1. Buscamos el asset estático del cubo básico
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CuboMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));

	// 2. Creamos el subobjeto y lo volvemos la raíz
	EscudoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EscudoMesh"));
	RootComponent = EscudoMesh;

	// 3. Verificamos si se encontró correctamente la malla y la asignamos
	if (CuboMesh.Succeeded())
	{
		EscudoMesh->SetStaticMesh(CuboMesh.Object);
	}
	SetActorScale3D(FVector(1.5f, 1.5f, 1.5f));

	VidaEscudo = 10.0f;
	EscudoMesh->SetCollisionProfileName(TEXT("BlockAll"));
}

void AEscudo::BeginPlay()
{
	Super::BeginPlay();
}

void AEscudo::RecibirDanoEscudo(float CantidadDano)
{
	VidaEscudo -= CantidadDano;

	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange,
			FString::Printf(TEXT("¡Escudo Impactado! Vida restante: %.0f"), VidaEscudo));
	}

	if (VidaEscudo <= 0.0f)
	{
		Destroy();
	}
}

