// Fill out your copyright notice in the Description page of Project Settings.

#include "Asteroides.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../Player/CentCosmosPawn.h"

AAsteroide::AAsteroide()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AsteroidMesh"));
    RootComponent = Mesh;

    // Tamaño: He añadido la escala grande aquí mismo
    Mesh->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("StaticMesh'/Game/Mallas_para_el_juego/Carp1/Meshy_AI_Cosmic_Vein_0607025636_texture_0.Meshy_AI_Cosmic_Vein_0607025636_texture_0'"));
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

void AAsteroide::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    // Si lo que golpeamos es la nave del jugador
    if (Other && Other->IsA(ACentCosmosPawn::StaticClass()))
    {
        // Aplicamos los 15 de daño brutal
        Cast<ACentCosmosPawn>(Other)->RecibirDanioNave(15.0f);

        // El asteroide se hace pedazos
        Destroy();
    }
}

