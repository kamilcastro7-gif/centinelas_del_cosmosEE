// Fill out your copyright notice in the Description page of Project Settings.

#include "ObstaculoNave.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AObstaculoNave::AObstaculoNave()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CuboMesh(TEXT("StaticMesh'/Game/Assetssss/A4/Meshy_AI_Cosmic_Breach_0607093056_texture.Meshy_AI_Cosmic_Breach_0607093056_texture'"));
	if (CuboMesh.Succeeded() && MallaObstaculo)
	{
		MallaObstaculo->SetStaticMesh(CuboMesh.Object);
		// Escala moderada para el obst�culo grande
		MallaObstaculo->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
		MallaObstaculo->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	}
}
void AObstaculoNave::InicializarObstaculo() {}