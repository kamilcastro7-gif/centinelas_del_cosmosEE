// Fill out your copyright notice in the Description page of Project Settings.
#include "SobreCargaApex.h"
#include "CentCosmosPawn.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Patterns/Decorator/EnemDecorador.h"

APowerUpSobrecargaApex::APowerUpSobrecargaApex()
{
	// Esfera verde — velocidad/poder
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FormaMesh(
		TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (FormaMesh.Succeeded() && MallaVisual)
	{
		MallaVisual->SetStaticMesh(FormaMesh.Object);
		MallaVisual->SetRelativeScale3D(FVector(0.4f));
	}
}

void APowerUpSobrecargaApex::AplicarEfecto(ACentCosmosPawn* Nave)
{
	if (!Nave) return;

	UEnemDecorador* Decorador = NewObject<UEnemDecorador>();
	if (Decorador)
	{
		Decorador->AplicarSobrecargaApex(Nave);
	}
}