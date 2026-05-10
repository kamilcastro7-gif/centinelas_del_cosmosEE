// Fill out your copyright notice in the Description page of Project Settings.
// EHeraldo_De_La_Ruina.cpp
#include "EHeraldo_De_La_Ruina.h"

AEHeraldo_De_La_Ruina::AEHeraldo_De_La_Ruina() {
    VelocidadHeraldo = 200.0f;
    DireccionMovimiento = FVector(1.0f, 0.0f, 0.0f);
    Tags.Add(FName("Enemigo"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
    if (MeshAsset.Succeeded()) MallaEnemigo->SetStaticMesh(MeshAsset.Object);
}

void AEHeraldo_De_La_Ruina::moverHeraldo() {
    FHitResult Hit;
    FVector Desplazamiento = DireccionMovimiento * VelocidadHeraldo * GetWorld()->GetDeltaSeconds();

    // El 'true' activa el Sweep para detectar paredes
    AddActorLocalOffset(Desplazamiento, true, &Hit);

    if (Hit.bBlockingHit) {
        // Si MirrorVector falla, intenta esta alternativa:
        DireccionMovimiento = DireccionMovimiento - 2 * FVector::DotProduct(DireccionMovimiento, Hit.Normal) * Hit.Normal;
        // O asegúrate que sea: DireccionMovimiento = FVector::MirrorVector(DireccionMovimiento, Hit.Normal);
    }
}