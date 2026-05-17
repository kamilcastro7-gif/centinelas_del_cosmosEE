// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilCarga.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "ProyectilBase.h"

AProyectilCarga::AProyectilCarga()
{
    PrimaryActorTick.bCanEverTick = false;

    // REFERENCIA: Shape_Sphere
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
    ProyectilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereCargaMesh"));
    RootComponent = ProyectilMesh;

    if (SphereMesh.Succeeded()) ProyectilMesh->SetStaticMesh(SphereMesh.Object);

    ProyectilMesh->SetCollisionProfileName(TEXT("Projectile"));
    ProyectilMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = ProyectilMesh;
    ProjectileMovement->InitialSpeed = 0.f;
    ProjectileMovement->MaxSpeed = 4000.f;
    ProjectileMovement->ProjectileGravityScale = 0.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bSimulationEnabled = false;

    DanoBase = 10.f;
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
    float VelocidadLanzamiento = 2500.f;

    if (TiempoCargaFinal >= 6.0f)
    {
        DanoBase = 100.f;
        VelocidadLanzamiento = 2000.f;
    }
    else if (TiempoCargaFinal >= 3.0f)
    {
        DanoBase = 45.f;
        VelocidadLanzamiento = 2700.f;
    }
    else
    {
        DanoBase = 10.f;
        VelocidadLanzamiento = 3400.f;
    }

    if (ProjectileMovement)
    {
        ProjectileMovement->bSimulationEnabled = true;
        ProjectileMovement->InitialSpeed = VelocidadLanzamiento;
        ProjectileMovement->Velocity = DireccionLanzamiento * VelocidadLanzamiento;
        ProjectileMovement->SetVelocityInLocalSpace(FVector(VelocidadLanzamiento, 0.f, 0.f));
        ProjectileMovement->UpdateComponentVelocity();
    }
    ProyectilMesh->OnComponentHit.AddDynamic(this, &AProyectilCarga::AlChocar);
}

void AProyectilCarga::AlChocar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        AProyectilBase* ProyectilEnemigo = Cast<AProyectilBase>(OtherActor);
        if (ProyectilEnemigo)
        {
            ProyectilEnemigo->RecibirImpacto();
            Destroy();
            return;
        }
    }
    Destroy();
}

