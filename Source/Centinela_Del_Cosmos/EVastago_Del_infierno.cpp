// Fill out your copyright notice in the Description page of Project Settings.
// EVastago_Del_infierno.cpp
#include "EVastago_Del_infierno.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProyectilBase.h"

AEVastago_Del_infierno::AEVastago_Del_infierno() {
    VelocidadVastago = 750.0f;
    DireccionMovimiento = FVector(1.0f, 1.0f, 0.0f);
    Tags.Add(FName("Enemigo"));

    TimerEstado = 0.0f;
    bEstaRodeando = false;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
    if (MeshAsset.Succeeded()) MallaEnemigo->SetStaticMesh(MeshAsset.Object);
}

void AEVastago_Del_infierno::BeginPlay() {
    Super::BeginPlay();

    // El vástago dispara cada 2 segundos
    FTimerHandle TimerHandle_Ataque;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Ataque, this, &AEVastago_Del_infierno::VasAtacar, 2.0f, true);
}

void AEVastago_Del_infierno::moverVastago() {
    AActor* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Jugador) return;

    float DeltaTime = GetWorld()->GetDeltaSeconds();
    TimerEstado += DeltaTime;

    float TiempoLimite = bEstaRodeando ? 7.0f : 5.0f;

    if (TimerEstado >= TiempoLimite) {
        TimerEstado = 0.0f;
        bEstaRodeando = !bEstaRodeando;
        // Elegimos dirección aleatoria para dispersarse
        DireccionDispersion = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal();
    }

    FVector Destino;
    if (bEstaRodeando) {
        // Lógica para rodear usando el ID único para variar el ángulo
        float Angulo = GetUniqueID() + (GetWorld()->GetTimeSeconds() * 0.8f);
        FVector Offset(FMath::Cos(Angulo) * 500.0f, FMath::Sin(Angulo) * 500.0f, 0.0f);
        Destino = Jugador->GetActorLocation() + Offset;
    }
    else {
        // Se dispersan en la dirección calculada
        Destino = GetActorLocation() + (DireccionDispersion * 400.0f);
    }

    // Interpolar posición para que el movimiento sea fluido y no saltos bruscos
    FVector NuevaPos = FMath::VInterpTo(GetActorLocation(), Destino, DeltaTime, 1.5f);
    SetActorLocation(NuevaPos);
}

void AEVastago_Del_infierno::VasAtacar() {
    AActor* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Jugador) return;

    float Distancia = FVector::Dist(GetActorLocation(), Jugador->GetActorLocation());

    // SOLO DISPARA SI ESTÁ CERCA (Ya rodeando al jugador)
    if (Distancia < 1100.0f) {
        FVector Ubicacion = GetActorLocation();
        // Forzamos a que el proyectil mire al jugador, no hacia arriba
        FRotator RotacionHaciaJugador = UKismetMathLibrary::FindLookAtRotation(Ubicacion, Jugador->GetActorLocation());

        AProyectilBase* Proy = GetWorld()->SpawnActor<AProyectilBase>(AProyectilBase::StaticClass(), Ubicacion, RotacionHaciaJugador);

        if (Proy) {
            Proy->MallaProyectil->SetRelativeScale3D(FVector(0.3f));
            // ALCANCE MUY CORTO: Se destruye en menos de 1 segundo
            Proy->InitialLifeSpan = 0.8f;

            if (Proy->MovimientoProyectil) {
                Proy->MovimientoProyectil->InitialSpeed = 1200.f;
                // Importante para que no salgan hacia arriba
                Proy->MovimientoProyectil->Velocity = RotacionHaciaJugador.Vector() * 1200.f;
            }
        }
    }
}
