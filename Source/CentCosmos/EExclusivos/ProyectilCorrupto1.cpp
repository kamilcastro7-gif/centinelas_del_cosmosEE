// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilCorrupto1.h"

AProyectilCorrupto1::AProyectilCorrupto1()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TorusMesh"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus'"));
    if (Asset.Succeeded()) Mesh->SetStaticMesh(Asset.Object);
    RootComponent = Mesh;
    Mesh->SetCollisionProfileName(TEXT("NoCollision")); // Evita choque al nacer

    Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    Movement->SetUpdatedComponent(RootComponent);
    Movement->bRotationFollowsVelocity = true;
    Movement->ProjectileGravityScale = 0.0f;
    InitialLifeSpan = 5.0f;
}



