// Fill out your copyright notice in the Description page of Project Settings.

#include "NucleoEter.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "CentCosmosPawn.h"

ANucleoEter::ANucleoEter()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> FormaMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Trim_90_In.Shape_Trim_90_In'"));
    if (FormaMesh.Succeeded() && MallaVisual)
    {
        MallaVisual->SetStaticMesh(FormaMesh.Object);
    }
}

void ANucleoEter::AplicarEfecto(ACentCosmosPawn* Nave)
{
    // Solo desaparece al pasar por él (El Destroy se ejecuta en la clase padre)
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("¡Nucleo Eter obtenido!"));
}