// Fill out your copyright notice in the Description page of Project Settings.

#include "ObstaculoRestos.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AObstaculoRestos::AObstaculoRestos()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CuboMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (CuboMesh.Succeeded() && MallaObstaculo)
	{
		MallaObstaculo->SetStaticMesh(CuboMesh.Object);
		// Escala peque�a para los escombros sueltos
		MallaObstaculo->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.2f));
		MallaObstaculo->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	}
}
void AObstaculoRestos::InicializarObstaculo() {}

