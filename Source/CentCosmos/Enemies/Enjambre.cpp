// Fill out your copyright notice in the Description page of Project Settings.

#include "Enjambre.h"
#include "Components/StaticMeshComponent.h"
#include "CentCosmosPawn.h" // Importante para detectar a la nave
#include "Engine/Engine.h"

AEnjambre::AEnjambre()
{
    PrimaryActorTick.bCanEverTick = true;
    MallaEnemigo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaEnemigo"));
    MallaEnemigo->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    RootComponent = MallaEnemigo;

    VidaActual = 10.0f;
    DanioDeChoque = 1.0f;
}

void AEnjambre::BeginPlay() { Super::BeginPlay(); }
void AEnjambre::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

// NUEVO: Daño por estrellarse contra el jugador
void AEnjambre::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
    {
        ACentCosmosPawn* NaveJugador = Cast<ACentCosmosPawn>(OtherActor);
        if (NaveJugador)
        {
            NaveJugador->RecibirDanioNave(DanioDeChoque);
        }
    }
}

// ACTUALIZADO: Con mensajes en pantalla y destrucción
void AEnjambre::RecibirDanioEnemigo(float Cantidad)
{
    VidaActual -= Cantidad;

    // Mostramos el daño en amarillo
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Enemigo recibio %f de dano. Vida restante: %f"), Cantidad, VidaActual));

    if (VidaActual <= 0.0f)
    {
        // Mensaje rojo de muerte
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("¡ENEMIGO DESTRUIDO!"));
        Destroy();
    }
}

