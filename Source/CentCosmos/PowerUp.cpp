// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerUp.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CentCosmosPawn.h" // Asegúrate de que sea el header real de tu nave

APowerUp::APowerUp()
{
    PrimaryActorTick.bCanEverTick = false;

    // 1. Configuración estricta de la colisión por código
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CajaColision"));
    RootComponent = CollisionBox;
    CollisionBox->SetBoxExtent(FVector(60.f, 60.f, 60.f));

    // Forzamos a que genere eventos de overlap de forma nativa
    CollisionBox->SetGenerateOverlapEvents(true);
    CollisionBox->SetCollisionProfileName(TEXT("Trigger"));

    // 2. Malla visual
    MallaVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaVisual"));
    MallaVisual->SetupAttachment(RootComponent);
    MallaVisual->SetCollisionProfileName(TEXT("NoCollision")); // Que la malla no estorbe
}

void APowerUp::BeginPlay()
{
    Super::BeginPlay();

    // Vinculamos la colisión de forma dinámica en el BeginPlay para asegurar el registro
    if (CollisionBox)
    {
        CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APowerUp::OnOverlap);
    }
}

void APowerUp::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        ACentCosmosPawn* Nave = Cast<ACentCosmosPawn>(OtherActor);
        if (Nave)
        {
            AplicarEfecto(Nave);
            Destroy(); // Desaparece inmediatamente del mapa
        }
    }
}

void APowerUp::AplicarEfecto(ACentCosmosPawn* Nave)
{
    // Base vacía
}

