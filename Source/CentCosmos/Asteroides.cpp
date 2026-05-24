// Fill out your copyright notice in the Description page of Project Settings.

#include "Asteroides.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "GameFramework/ProjectileMovementComponent.h"

AAsteroide::AAsteroide()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AsteroidMesh"));
    RootComponent = Mesh;

    // Tamaño: He añadido la escala grande aquí mismo
    Mesh->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus'"));
    if (Asset.Succeeded()) Mesh->SetStaticMesh(Asset.Object);

    // Configuración de Movimiento
    Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    Movement->bShouldBounce = true;
    Movement->Bounciness = 0.8f;
    Movement->Friction = 0.0f;
    Movement->ProjectileGravityScale = 0.0f;

    // Configuración de Colisiones
    Mesh->SetCollisionProfileName(TEXT("BlockAll"));
    Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);

    // Activamos solapamientos para detectar al jugador
    Mesh->SetGenerateOverlapEvents(true);

    PrimaryActorTick.bCanEverTick = true;
}

void AAsteroide::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(20.0f);

    // Cambiar dirección cada 3 segundos
    GetWorldTimerManager().SetTimer(TimerCambioDireccion, this, &AAsteroide::CambiarDireccion, 3.0f, true);

    // Dirección inicial inmediata
    CambiarDireccion();
}

void AAsteroide::CambiarDireccion()
{
    FVector NuevaDir = FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal();
    Movement->Velocity = NuevaDir * 500.0f;
}

void AAsteroide::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AddActorLocalRotation(FRotator(1.0f, 2.0f, 0.5f));
}

// Asegúrate de tener esta función si quieres que se destruya al tocar al jugador
void AAsteroide::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        Destroy();
    }
}

