// Fill out your copyright notice in the Description page of Project Settings.

#include "ObstaculoSatelite.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AObstaculoSatelite::AObstaculoSatelite()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CuboMesh(TEXT("StaticMesh'/Game/Mallas_para_el_juego/Carp1/Meshy_AI_Drifting_Wreckage_in__0607094119_texture.Meshy_AI_Drifting_Wreckage_in__0607094119_texture'"));
	if (CuboMesh.Succeeded() && MallaObstaculo)
	{
		MallaObstaculo->SetStaticMesh(CuboMesh.Object);
		// Escala mediana
		MallaObstaculo->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
		MallaObstaculo->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	}
}
void AObstaculoSatelite::InicializarObstaculo() {}