// Fill out your copyright notice in the Description page of Project Settings.


#include "EVigia_Del_Vacio.h"
#include "ProyectilBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

#include "Kismet/GameplayStatics.h"      
#include "Kismet/KismetMathLibrary.h"  

AEVigia_Del_Vacio::AEVigia_Del_Vacio() {
    // Definimos los parámetros de la oscilación
    VelocidadVigia = 2.0f; // Frecuencia (más alto = más rápido de lado a lado)
    AmplitudPatrulla = 500.0f; // Se moverá 500cm a la izquierda y 500cm a la derecha
    Tags.Add(FName("Enemigo"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus'"));
    if (MeshAsset.Succeeded()) {
        MallaEnemigo->SetStaticMesh(MeshAsset.Object);
    }

    // DESACTIVAMOS COLISIONES DE MOVIMIENTO para que nada lo frene
    // Seguirá teniendo colisión para que el jugador le dispare, pero no para frenarse
    MallaEnemigo->SetSimulatePhysics(false);
    MallaEnemigo->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AEVigia_Del_Vacio::BeginPlay() {
    Super::BeginPlay();
    // Guardamos dónde nació para oscilar alrededor de ese punto
    PosicionInicialSpawn = GetActorLocation();
    FTimerHandle TimerHandle_Ataque;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Ataque, this, &AEVigia_Del_Vacio::VigAtacar, 5.0f, true);
}

void AEVigia_Del_Vacio::moverVigia() {
    // Usamos el tiempo para oscilar el ángulo del arco
    float Tiempo = GetWorld()->GetTimeSeconds() * VelocidadVigia;
    float Radio = 1200.0f; // Distancia larga de francotirador

    // El arco se mueve entre -1.5 y 1.5 radianes (aprox una media luna)
    float OffsetAngulo = FMath::Sin(Tiempo) * 1.5f;

    FVector NuevaPos;
    NuevaPos.X = PosicionInicialSpawn.X + (FMath::Cos(OffsetAngulo) * Radio);
    NuevaPos.Y = PosicionInicialSpawn.Y + (FMath::Sin(OffsetAngulo) * Radio);
    NuevaPos.Z = PosicionInicialSpawn.Z;

    SetActorLocation(NuevaPos);

    // Siempre mirar al jugador para preparar el disparo
    AActor* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Jugador) {
        FRotator RotacionAlPlayer = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Jugador->GetActorLocation());
        SetActorRotation(RotacionAlPlayer);
    }
}

void AEVigia_Del_Vacio::VigAtacar() {
    if (!GetWorld()) return;

    FVector Ubicacion = GetActorLocation() + (GetActorForwardVector() * 120.f);

    // Apuntar a la posición actual del jugador para que el disparo tenga dirección
    AActor* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    FRotator Rotacion = (Jugador) ?
        UKismetMathLibrary::FindLookAtRotation(Ubicacion, Jugador->GetActorLocation()) : GetActorRotation();

    AProyectilBase* Proy = GetWorld()->SpawnActor<AProyectilBase>(AProyectilBase::StaticClass(), Ubicacion, Rotacion);

    if (Proy) {
        Proy->MallaProyectil->SetRelativeScale3D(FVector(0.8f)); // Redondo y mediano
        Proy->InitialLifeSpan = 5.0f; // Se destruye si falla

        if (Proy->MovimientoProyectil) {
            // Le damos velocidad inicial en la dirección a la que apunta
            Proy->MovimientoProyectil->InitialSpeed = 6000.f;
            Proy->MovimientoProyectil->MaxSpeed = 6000.f;
            Proy->MovimientoProyectil->Velocity = Rotacion.Vector() * 6000.f;
        }
    }
}


