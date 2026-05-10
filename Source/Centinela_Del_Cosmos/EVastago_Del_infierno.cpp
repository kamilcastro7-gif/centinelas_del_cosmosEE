// Fill out your copyright notice in the Description page of Project Settings.
// EVastago_Del_infierno.cpp
#include "EVastago_Del_infierno.h"
#include "UObject/ConstructorHelpers.h"

AEVastago_Del_infierno::AEVastago_Del_infierno() {
    VelocidadVastago = 750.0f;
    DireccionMovimiento = FVector(1.0f, 1.0f, 0.0f);
    Tags.Add(FName("Enemigo"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
    if (MeshAsset.Succeeded()) MallaEnemigo->SetStaticMesh(MeshAsset.Object);
}

void AEVastago_Del_infierno::moverVastago() {
    FHitResult Hit;
    FVector Desplazamiento = DireccionMovimiento * VelocidadVastago * GetWorld()->GetDeltaSeconds();

    // El 'true' activa el Sweep para detectar paredes
    AddActorLocalOffset(Desplazamiento, true, &Hit);

    if (Hit.bBlockingHit) {
        // Si MirrorVector falla, intenta esta alternativa:
        DireccionMovimiento = DireccionMovimiento - 2 * FVector::DotProduct(DireccionMovimiento, Hit.Normal) * Hit.Normal;
        // O asegúrate que sea: DireccionMovimiento = FVector::MirrorVector(DireccionMovimiento, Hit.Normal);
    }
}