// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilCorrupto2.h"

AProyectilCorrupto2::AProyectilCorrupto2()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
    if (Asset.Succeeded()) Mesh->SetStaticMesh(Asset.Object);
    Mesh->SetRelativeScale3D(FVector(2.5f));
    RootComponent = Mesh;
    Mesh->SetCollisionProfileName(TEXT("NoCollision")); // Evita choque al nacer

    Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    Movement->SetUpdatedComponent(RootComponent);
    Movement->bRotationFollowsVelocity = true;
    Movement->ProjectileGravityScale = 0.0f;
    InitialLifeSpan = 5.0f;
}

