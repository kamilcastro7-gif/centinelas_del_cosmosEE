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
#include "../EExclusivo.h"
#include "TribunalBase/TribunalBase.h"
#include "Escudo.h"
#include "TribunalBase/PlacaMetal.h"
// --- NUEVOS INCLUDES ---
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AProyectilCarga::AProyectilCarga()
{
    PrimaryActorTick.bCanEverTick = false;

    // 1. HITBOX INVISIBLE
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
        TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

    ProyectilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereCargaMesh"));
    RootComponent = ProyectilMesh;

    if (SphereMesh.Succeeded()) ProyectilMesh->SetStaticMesh(SphereMesh.Object);

    // Ocultamos la esfera
    ProyectilMesh->SetHiddenInGame(true);
    ProyectilMesh->SetCollisionProfileName(TEXT("Projectile"));
    ProyectilMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

    // 2. EFECTO VISUAL NIAGARA
    EfectoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoNiagara"));
    EfectoNiagara->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Hit_4.NS_Hit_4'"));
    if (NiagaraAsset.Succeeded())
    {
        EfectoNiagara->SetAsset(NiagaraAsset.Object);
    }

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = ProyectilMesh;
    ProjectileMovement->InitialSpeed = 0.f;
    ProjectileMovement->MaxSpeed = 5000.f;
    ProjectileMovement->ProjectileGravityScale = 0.f;
    ProjectileMovement->bRotationFollowsVelocity = true;

    ProjectileMovement->SetActive(false);

    DanoBase = 0.5f;
}

// ... (El resto de tus funciones InicializarCarga, LiberarProyectil y AlChocar se mantienen exactamente igual) ...

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
    // ¡NUEVO! Le damos sus 4 segundos de vida justo cuando sale disparado
    SetLifeSpan(4.0f);

    float VelocidadLanzamiento;

    if (TiempoCargaFinal >= 2.8f)
    {
        DanoBase = 15.f;
        VelocidadLanzamiento = 2000.f;
    }
    else if (TiempoCargaFinal >= 1.2f)
    {
        DanoBase = 7.f;
        VelocidadLanzamiento = 2700.f;
    }
    else
    {
        DanoBase = 0.5f;
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
        if (EnemigoImpactado) { EnemigoImpactado->RecibirDanioEnemigo(DanoBase); Destroy(); return; }

        AEExclusivo* EnemigoExclusivo = Cast<AEExclusivo>(OtherActor);
        if (EnemigoExclusivo) { EnemigoExclusivo->RecibirDanoEnemigo(DanoBase); Destroy(); return; }

        ATribunalBase* Tribunal = Cast<ATribunalBase>(OtherActor);
        if (Tribunal) { Tribunal->RecibirDanioJefe(DanoBase); Destroy(); return; }

        APlacaMetal* Placa = Cast<APlacaMetal>(OtherActor);
        if (Placa) { Placa->RecibirDanioPlaca(DanoBase); Destroy(); return; }

        AEscudo* Escudo = Cast<AEscudo>(OtherActor);
        if (Escudo) { Escudo->RecibirDanoEscudo(DanoBase); Destroy(); return; }

        AGrietaAntimateria* Grieta = Cast<AGrietaAntimateria>(OtherActor);
        if (Grieta) { Grieta->ProcesarImpacto(); Destroy(); return; }
    }

    Destroy();
}

