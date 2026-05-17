// Fill out your copyright notice in the Description page of Project Settings.

#include "ObstaculoSatelite.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AObstaculoSatelite::AObstaculoSatelite()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CuboMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (CuboMesh.Succeeded() && MallaObstaculo)
	{
		MallaObstaculo->SetStaticMesh(CuboMesh.Object);
		// Escala mediana
		MallaObstaculo->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	}
}
void AObstaculoSatelite::InicializarObstaculo() {}