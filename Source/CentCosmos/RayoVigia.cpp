// Fill out your copyright notice in the Description page of Project Settings.


#include "RayoVigia.h"
#include "DrawDebugHelpers.h"
#include "UObject/ConstructorHelpers.h"

ARayoVigia::ARayoVigia()
{
    PrimaryActorTick.bCanEverTick = false;
    LongitudMaxima = 500.f;

    USceneComponent* EscenaRaiz = CreateDefaultSubobject<USceneComponent>(TEXT("EscenaRaiz"));
    RootComponent = EscenaRaiz;

    MallaRayo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaRayo"));
    MallaRayo->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
    if (CubeAsset.Succeeded())
    {
        MallaRayo->SetStaticMesh(CubeAsset.Object);
    }

    MallaRayo->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MallaRayo->SetCollisionResponseToAllChannels(ECR_Ignore);

    MallaRayo->SetHiddenInGame(false);
    SetActorHiddenInGame(false);
}

void ARayoVigia::BeginPlay()
{
    Super::BeginPlay();
}

void ARayoVigia::ActualizarLongitudRayo()
{
    if (!MallaRayo) return;

    MallaRayo->SetHiddenInGame(false);

    FVector OriginLocation = MallaRayo->GetComponentLocation();
    FVector DireccionDisparo = MallaRayo->GetRightVector();

    FVector StartLocation = OriginLocation + (DireccionDisparo * 150.f);
    FVector EndLocation = StartLocation + (DireccionDisparo * LongitudMaxima);

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;

    CollisionParams.AddIgnoredActor(this);
    if (GetOwner())
    {
        CollisionParams.AddIgnoredActor(GetOwner());
        TArray<AActor*> Hijos;
        GetOwner()->GetAttachedActors(Hijos);
        CollisionParams.AddIgnoredActors(Hijos);
    }

    float LongitudActual = LongitudMaxima;

    if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_WorldStatic, CollisionParams))
    {
        LongitudActual = HitResult.Distance + 150.f;
    }

    float FactorEscalaY = LongitudActual / 100.f;

    MallaRayo->SetRelativeScale3D(FVector(0.15f, FactorEscalaY, 0.15f));
    MallaRayo->SetRelativeLocation(FVector(0.f, LongitudActual * 0.5f, 0.f));
}

