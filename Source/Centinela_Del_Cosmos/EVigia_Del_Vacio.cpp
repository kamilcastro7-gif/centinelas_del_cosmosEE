// Fill out your copyright notice in the Description page of Project Settings.


#include "EVigia_Del_Vacio.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

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
}

void AEVigia_Del_Vacio::moverVigia() {
    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    // MATEMÁTICAS PURAS: Calculamos la nueva posición Y basada en el tiempo
    // Usamos Sinus para un movimiento suave de vaivén
    float Tiempo = Mundo->GetTimeSeconds();

    // NuevaY = SpawnY + Amplitud * Sin(Tiempo * Frecuencia)
    float NuevaY = PosicionInicialSpawn.Y + (AmplitudPatrulla * FMath::Sin(Tiempo * VelocidadVigia));

    // Mantenemos X y Z igual que en el spawn
    FVector NuevaPosicion = FVector(PosicionInicialSpawn.X, NuevaY, PosicionInicialSpawn.Z);

    // Teletransportamos suavemente al actor a la nueva posición cada frame
    // El 'false' desactiva el sweep, obligándolo a estar ahí
    SetActorLocation(NuevaPosicion, false);
}