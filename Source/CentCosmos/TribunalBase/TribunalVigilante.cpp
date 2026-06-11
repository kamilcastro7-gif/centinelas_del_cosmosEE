// Fill out your copyright notice in the Description page of Project Settings.

#include "TribunalVigilante.h"
#include "RayoVigia.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ATribunalVigilante::ATribunalVigilante()
{
    Vida = 120.0f;
    PrimaryActorTick.bCanEverTick = true;
    VelocidadGiro = 60.f;
    RangoDeteccion = 2200.f;
    bEstaActivo = false;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PyramidAsset(TEXT("StaticMesh'/Game/Assetssss/A10/Meshy_AI_Orbital_Pyramid_of_Li_0607221452_texture.Meshy_AI_Orbital_Pyramid_of_Li_0607221452_texture'"));
    if (PyramidAsset.Succeeded() && MallaTribunal)
    {
        MallaTribunal->SetStaticMesh(PyramidAsset.Object);
    }

    if (MallaTribunal)
    {
        MallaTribunal->SetRelativeScale3D(FVector(4.f, 4.f, 4.f));
    }

    ComponenteRayo = CreateDefaultSubobject<UChildActorComponent>(TEXT("ComponenteRayo"));
    ComponenteRayo->SetupAttachment(RootComponent);
    ComponenteRayo->SetChildActorClass(ARayoVigia::StaticClass());
    ComponenteRayo->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
}

void ATribunalVigilante::BeginPlay()
{
    Super::BeginPlay();
}

void ATribunalVigilante::ActivarJefe()
{
    APawn* NaveJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (NaveJugador)
    {
        float DistanciaAlJugador = FVector::Dist2D(GetActorLocation(), NaveJugador->GetActorLocation());
        if (DistanciaAlJugador <= RangoDeteccion)
        {
            bEstaActivo = true;
        }
    }
}

void ATribunalVigilante::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    APawn* NaveJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (NaveJugador)
    {
        float DistanciaAlJugador = FVector::Dist2D(GetActorLocation(), NaveJugador->GetActorLocation());

        if (DistanciaAlJugador <= RangoDeteccion)
        {
            bEstaActivo = true;
        }
        else
        {
            bEstaActivo = false;
        }
    }

    if (bEstaActivo)
    {
        FRotator NuevaRotacion = GetActorRotation();
        NuevaRotacion.Yaw += VelocidadGiro * DeltaTime;
        SetActorRotation(NuevaRotacion);

        if (ComponenteRayo)
        {
            ARayoVigia* ElRayo = Cast<ARayoVigia>(ComponenteRayo->GetChildActor());
            if (ElRayo)
            {
                ElRayo->SetActorHiddenInGame(false);
                ElRayo->SetActorEnableCollision(true);
                ElRayo->ActualizarLongitudRayo();
            }
        }
    }
    else
    {
        if (ComponenteRayo)
        {
            ARayoVigia* ElRayo = Cast<ARayoVigia>(ComponenteRayo->GetChildActor());
            if (ElRayo)
            {
                ElRayo->SetActorHiddenInGame(true);
                ElRayo->SetActorEnableCollision(false);

                if (ElRayo->MallaRayo)
                {
                    ElRayo->MallaRayo->SetHiddenInGame(true);
                }
            }
        }
    }
}
