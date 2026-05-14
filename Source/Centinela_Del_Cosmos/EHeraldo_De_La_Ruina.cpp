// Fill out your copyright notice in the Description page of Project Settings.
// EHeraldo_De_La_Ruina.cpp

#include "EHeraldo_De_La_Ruina.h"
#include "Kismet/GameplayStatics.h"

AEHeraldo_De_La_Ruina::AEHeraldo_De_La_Ruina() {
    VelocidadHeraldo = 350.0f;
    bEstaPersiguiendo = true;
    TimerEstadoHeraldo = 0.0f;
    Tags.Add(FName("Enemigo"));

    // Volvemos a la forma que funcionaba para cargar el cubo
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
    if (MeshAsset.Succeeded()) {
        MallaEnemigo->SetStaticMesh(MeshAsset.Object);
    }
}

void AEHeraldo_De_La_Ruina::moverHeraldo()
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    AActor* Jugador = UGameplayStatics::GetPlayerPawn(Mundo, 0);
    if (!Jugador) return;

    float DeltaTime = Mundo->GetDeltaSeconds();
    TimerEstadoHeraldo += DeltaTime;

    // 1. GESTIÓN DE ESTADOS (15s Persecución / 3s Descanso)
    float TiempoLimite = bEstaPersiguiendo ? 15.0f : 3.0f;

    if (TimerEstadoHeraldo >= TiempoLimite)
    {
        TimerEstadoHeraldo = 0.0f;
        bEstaPersiguiendo = !bEstaPersiguiendo;
    }

    // Si está descansando, salimos de la función
    if (!bEstaPersiguiendo) return;

    // 2. CÁLCULO DE DIRECCIÓN
    FVector MiPosicion = GetActorLocation();
    FVector PosicionJugador = Jugador->GetActorLocation();

    FVector Direccion = (PosicionJugador - MiPosicion);
    Direccion.Z = 0.0f; // Mantenerlo siempre pegado al suelo

    if (Direccion.Size() > 10.0f)
    {
        FVector DireccionNormal = Direccion.GetSafeNormal();
        FVector Desplazamiento = DireccionNormal * VelocidadHeraldo * DeltaTime;

        // 3. MOVIMIENTO CON TRATAMIENTO DE COLISIONES
        FHitResult Hit;

        // Intentamos movernos con Sweep (true) para detectar colisiones
        AddActorWorldOffset(Desplazamiento, true, &Hit);

        // Si hay un bloqueo, verificamos con qué chocamos
        if (Hit.bBlockingHit)
        {
            AActor* Obstaculo = Hit.GetActor();

            // Si el obstáculo es un Enemigo (Vástago o Vigía), lo ignoramos para no quedarnos quietos
            if (Obstaculo && Obstaculo->ActorHasTag(FName("Enemigo")))
            {
                // Movemos sin Sweep (false) para atravesar o empujar al aliado
                AddActorWorldOffset(Desplazamiento, false);
            }
            else
            {
                // Si es una pared o el jugador, intentamos un pequeño deslizamiento lateral
                // para que no se quede pegado a las esquinas
                FVector Deslizamiento = FVector::VectorPlaneProject(Desplazamiento, Hit.Normal);
                AddActorWorldOffset(Deslizamiento, true);
            }
        }

        // 4. ACTUALIZAR ROTACIÓN
        SetActorRotation(DireccionNormal.Rotation());
    }
}