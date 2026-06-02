// Fill out your copyright notice in the Description page of Project Settings.
#include "NucleoEter.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "CentCosmosPawn.h"
#include "Engine/Engine.h"

ANucleoEter::ANucleoEter()
{
    CantidadCuracion = 50.0f;

    // Cubo cian — curacion
    static ConstructorHelpers::FObjectFinder<UStaticMesh> FormaMesh(
        TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
    if (FormaMesh.Succeeded() && MallaVisual)
    {
        MallaVisual->SetStaticMesh(FormaMesh.Object);
        MallaVisual->SetRelativeScale3D(FVector(0.3f));
    }
}

void ANucleoEter::AplicarEfecto(ACentCosmosPawn* Nave)
{
    if (!Nave) return;

    // RestaurarVida conecta con el Decorator y notifica al Observer
    Nave->RestaurarVida(CantidadCuracion);
}