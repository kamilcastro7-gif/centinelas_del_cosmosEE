// Fill out your copyright notice in the Description page of Project Settings.

#include "MunicionDispersion.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "CentCosmosPawn.h"
#include "TimerManager.h"

AMunicionDispersion::AMunicionDispersion()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> FormaMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_WideCapsule.Shape_WideCapsule'"));
    if (FormaMesh.Succeeded() && MallaVisual)
    {
        MallaVisual->SetStaticMesh(FormaMesh.Object);
    }
}

void AMunicionDispersion::AplicarEfecto(ACentCosmosPawn* Nave)
{
    if (!Nave) return;

    Nave->bTieneDisparoTriple = true;

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("¡MUNICIÓN DISPERSIÓN: Disparo triple activo por 10 segundos!"));

    // Iniciamos el reloj de 10 segundos para llamar a la desactivación en la nave
    FTimerHandle TimerDispersion;
    Nave->GetWorld()->GetTimerManager().SetTimer(TimerDispersion, Nave, &ACentCosmosPawn::DesactivarDisparoTriple, 10.0f, false);
}