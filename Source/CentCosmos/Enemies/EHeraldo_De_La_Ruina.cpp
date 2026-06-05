// Fill out your copyright notice in the Description page of Project Settings.

#include "EHeraldo_De_La_Ruina.h"
#include "Kismet/GameplayStatics.h"
#include "ProyectilBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AEHeraldo_De_La_Ruina::AEHeraldo_De_La_Ruina() {
    VelocidadHeraldo = 350.0f;
    bEstaPersiguiendo = true;
    TimerEstadoHeraldo = 0.0f;
    Tags.Add(FName("Enemigo"));

    VidaActual = 40.0f;
    DanioDeChoque = 5.0f;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
    if (MeshAsset.Succeeded()) {
        MallaEnemigo->SetStaticMesh(MeshAsset.Object);
    }
}

void AEHeraldo_De_La_Ruina::BeginPlay() {
    Super::BeginPlay();
    FTimerHandle TimerHandle_Ataque;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Ataque, this, &AEHeraldo_De_La_Ruina::HerAtacar, 10.0f, true);
}

void AEHeraldo_De_La_Ruina::moverHeraldo()
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    AActor* Jugador = UGameplayStatics::GetPlayerPawn(Mundo, 0);
    if (!Jugador) return;

    float DeltaTime = Mundo->GetDeltaSeconds();
    TimerEstadoHeraldo += DeltaTime;

    float TiempoLimite = bEstaPersiguiendo ? 15.0f : 3.0f;

    if (TimerEstadoHeraldo >= TiempoLimite)
    {
        TimerEstadoHeraldo = 0.0f;
        bEstaPersiguiendo = !bEstaPersiguiendo;
    }

    if (!bEstaPersiguiendo) return;

    FVector MiPosicion = GetActorLocation();
    FVector PosicionJugador = Jugador->GetActorLocation();

    FVector Direccion = (PosicionJugador - MiPosicion);
    Direccion.Z = 0.0f;

    if (Direccion.Size() > 10.0f)
    {
        FVector DireccionNormal = Direccion.GetSafeNormal();
        FVector Desplazamiento = DireccionNormal * VelocidadHeraldo * DeltaTime;

        FHitResult Hit;
        AddActorWorldOffset(Desplazamiento, true, &Hit);

        if (Hit.bBlockingHit)
        {
            AActor* Obstaculo = Hit.GetActor();

            if (Obstaculo && Obstaculo->ActorHasTag(FName("Enemigo")))
            {
                AddActorWorldOffset(Desplazamiento, false);
            }
            else
            {
                FVector Deslizamiento = FVector::VectorPlaneProject(Desplazamiento, Hit.Normal);
                AddActorWorldOffset(Deslizamiento, true);
            }
        }

        SetActorRotation(DireccionNormal.Rotation());
    }
}

void AEHeraldo_De_La_Ruina::HerAtacar() {
    if (!GetWorld()) return;

    AActor* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Jugador) return;

    FVector Ubicacion = GetActorLocation() + (GetActorForwardVector() * 200.f);
    FRotator Rotacion = GetActorRotation();

    AProyectilBase* GranEsfera = GetWorld()->SpawnActor<AProyectilBase>(AProyectilBase::StaticClass(), Ubicacion, Rotacion);

    if (GranEsfera) {
        GranEsfera->bEsSeguidor = true;
        GranEsfera->Danio = 5.0f;      // Mantiene sus 5 puntos de daño a la nave

        // =========================================================================
        // NUEVO BALANCE: Desaparece automáticamente tras 6 segundos de persecución
        // =========================================================================
        GranEsfera->SetLifeSpan(6.0f);

        if (GranEsfera->MovimientoProyectil) {
            // =========================================================================
            // NUEVO BALANCE: Mayor velocidad inicial y máxima para presionar al jugador
            // =========================================================================
            GranEsfera->MovimientoProyectil->InitialSpeed = 950.f;
            GranEsfera->MovimientoProyectil->MaxSpeed = 1200.f;
            GranEsfera->MovimientoProyectil->bShouldBounce = false;

            // Sistema Homing de Persecución Persistente
            GranEsfera->MovimientoProyectil->bIsHomingProjectile = true;
            GranEsfera->MovimientoProyectil->HomingTargetComponent = Jugador->GetRootComponent();

            // Fuerza de aceleración del giro para que no pierda el rastro a altas velocidades
            GranEsfera->MovimientoProyectil->HomingAccelerationMagnitude = 4500.f;
        }
    }
}