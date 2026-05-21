// Fill out your copyright notice in the Description page of Project Settings.

#include "SobreCargaApex.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "CentCosmosPawn.h"
#include "TimerManager.h"

ASobreCargaApex::ASobreCargaApex()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> FormaMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'"));
    if (FormaMesh.Succeeded() && MallaVisual)
    {
        MallaVisual->SetStaticMesh(FormaMesh.Object);
    }
}

void ASobreCargaApex::AplicarEfecto(ACentCosmosPawn* Nave)
{
    if (!Nave) return;

    Nave->bTieneSobreCargaApex = true;

    // Incremento brutal de agilidad para que se note el cambio de inmediato
    Nave->MoveSpeed = Nave->MoveSpeedBase * 1.60f; // +60% de velocidad de traslación real
    Nave->FireRate = Nave->FireRateBase * 0.50f;   // Dispara el doble de rápido

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("¡SOBRECARGA APEX: Agilidad y Proyectiles veloces por 8 segundos!"));

    // Iniciamos el reloj de 8 segundos para restaurar los valores iniciales de la nave
    FTimerHandle TimerApex;
    Nave->GetWorld()->GetTimerManager().SetTimer(TimerApex, Nave, &ACentCosmosPawn::DesactivarSobreCargaApex, 8.0f, false);
}