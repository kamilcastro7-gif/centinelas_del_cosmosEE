// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilCarga.h"
#include "Kismet/GameplayStatics.h" 
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "CentCosmosPawn.h"
#include "ProyectilBase.h"
#include "GrietaAntimateria.h" 
#include "Enjambre.h" 

AProyectilCarga::AProyectilCarga()
{
    PrimaryActorTick.bCanEverTick = false;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
        TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

    ProyectilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereCargaMesh"));
    RootComponent = ProyectilMesh;

    if (SphereMesh.Succeeded()) ProyectilMesh->SetStaticMesh(SphereMesh.Object);

    ProyectilMesh->SetCollisionProfileName(TEXT("Projectile"));
    ProyectilMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = ProyectilMesh;
    ProjectileMovement->InitialSpeed = 0.f;
    ProjectileMovement->MaxSpeed = 5000.f;
    ProjectileMovement->ProjectileGravityScale = 0.f;
    ProjectileMovement->bRotationFollowsVelocity = true;

    ProjectileMovement->SetActive(false);

    DanoBase = 2.f;
    InitialLifeSpan = 4.0f;
}

void AProyectilCarga::InicializarCarga(float TiempoCarga)
{
    if (TiempoCarga >= 6.0f) SetActorScale3D(FVector(1.8f));
    else if (TiempoCarga >= 3.0f) SetActorScale3D(FVector(1.2f));
    else
    {
        float ProgresoEscala = FMath::Lerp(0.4f, 1.0f, TiempoCarga / 3.0f);
        SetActorScale3D(FVector(ProgresoEscala));
    }
}

void AProyectilCarga::LiberarProyectil(float TiempoCargaFinal, FVector DireccionLanzamiento)
{
    float VelocidadLanzamiento;

    // 1. BALANCE ACTUALIZADO DE DAÑO
    if (TiempoCargaFinal >= 6.0f)
    {
        DanoBase = 20.f; // Daño brutal para la carga máxima (opcional, premia aguantar 6s)
        VelocidadLanzamiento = 2000.f;
    }
    else if (TiempoCargaFinal >= 3.0f)
    {
        DanoBase = 10.f; // SOLICITADO: La carga media ahora hace 10 de daño
        VelocidadLanzamiento = 2700.f;
    }
    else
    {
        DanoBase = 2.f;  // Daño Mínimo
        VelocidadLanzamiento = 3400.f;
    }

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        ACentCosmosPawn* NaveJugador = Cast<ACentCosmosPawn>(PlayerPawn);
        if (NaveJugador && NaveJugador->bRalentizadoPorChispa)
        {
            VelocidadLanzamiento = VelocidadLanzamiento * 0.5f;
        }
    }

    if (ProjectileMovement)
    {
        FVector DirNormal = DireccionLanzamiento.GetSafeNormal();

        ProjectileMovement->SetActive(true);
        ProjectileMovement->MaxSpeed = VelocidadLanzamiento;
        ProjectileMovement->InitialSpeed = VelocidadLanzamiento;
        ProjectileMovement->Velocity = DirNormal * VelocidadLanzamiento;
        ProjectileMovement->UpdateComponentVelocity();
    }

    ProyectilMesh->OnComponentHit.AddDynamic(this, &AProyectilCarga::AlChocar);
}

void AProyectilCarga::AlChocar(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        AEnjambre* EnemigoImpactado = Cast<AEnjambre>(OtherActor);
        if (EnemigoImpactado)
        {
            EnemigoImpactado->RecibirDanioEnemigo(DanoBase);
            Destroy();
            return;
        }

        AProyectilBase* ProyectilEnemigo = Cast<AProyectilBase>(OtherActor);
        if (ProyectilEnemigo)
        {
            ProyectilEnemigo->RecibirImpacto();
            Destroy();
            return;
        }

        AGrietaAntimateria* Grieta = Cast<AGrietaAntimateria>(OtherActor);
        if (Grieta)
        {
            Grieta->ProcesarImpacto();
            Destroy();
            return;
        }
    }

    Destroy();
}

