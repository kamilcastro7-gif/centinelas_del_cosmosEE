// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacaMetal.h"
#include "TribAsedio.h" // Actualizado al nuevo nombre
#include "UObject/ConstructorHelpers.h"

APlacaMetal::APlacaMetal()
{
    PrimaryActorTick.bCanEverTick = false;

    MallaPlaca = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPlaca"));
    RootComponent = MallaPlaca;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
    if (SphereAsset.Succeeded())
    {
        MallaPlaca->SetStaticMesh(SphereAsset.Object);
    }

    MallaPlaca->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaPlaca->SetRelativeScale3D(FVector(2.2f, 2.2f, 2.2f));

    VidaMax = 10.f;
    VidaActual = VidaMax;
}

void APlacaMetal::BeginPlay()
{
    Super::BeginPlay();
    VidaActual = VidaMax;
}

void APlacaMetal::RecibirDanioPlaca(float CantidadDanio)
{
    VidaActual -= CantidadDanio;
    if (VidaActual <= 0.f)
    {
        // Casteo actualizado a la nueva clase TribAsedio
        ATribAsedio* Jefe = Cast<ATribAsedio>(OwnerJefe);
        if (Jefe)
        {
            Jefe->EliminarPlacaDeArreglo(this);
        }
        Destroy();
    }
}

