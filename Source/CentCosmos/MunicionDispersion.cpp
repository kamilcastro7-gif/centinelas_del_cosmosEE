// Fill out your copyright notice in the Description page of Project Settings.
#include "MunicionDispersion.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "CentCosmosPawn.h"
#include "Engine/Engine.h"

AMunicionDispersion::AMunicionDispersion()
{
	// Cubo naranja — municion de dispersion
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FormaMesh(
		TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (FormaMesh.Succeeded() && MallaVisual)
	{
		MallaVisual->SetStaticMesh(FormaMesh.Object);
		MallaVisual->SetRelativeScale3D(FVector(0.3f));
	}
}

void AMunicionDispersion::AplicarEfecto(ACentCosmosPawn* Nave)
{
	if (!Nave) return;

	// Activa disparo triple en la nave
	Nave->bTieneDisparoTriple = true;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange,
			TEXT("Disparo Triple Activado!"));
	}
}