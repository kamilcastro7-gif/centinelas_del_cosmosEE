// Fill out your copyright notice in the Description page of Project Settings.

#include "FragmentoErupcion.h"
#include "Components/StaticMeshComponent.h"
#include "RastrosFuego.h" 
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h" // NECESARIO para que GetWorldTimerManager funcione

AFragmentoErupcion::AFragmentoErupcion()
{
    PrimaryActorTick.bCanEverTick = true;

    FragmentoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FragmentoMesh"));
    RootComponent = FragmentoMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    if (CubeMesh.Succeeded()) FragmentoMesh->SetStaticMesh(CubeMesh.Object);

    FragmentoMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    SetActorScale3D(FVector(2.5f, 2.5f, 2.5f));

    Velocidad = 900.0f;
}

void AFragmentoErupcion::BeginPlay()
{
    Super::BeginPlay();

    APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (PlayerPawn)
    {
        // Guardamos la posición exacta del jugador en este momento preciso
        FVector PosicionJugador = PlayerPawn->GetActorLocation();

        // Calculamos el vector dirección apuntando hacia esa posición fija
        DireccionVuelo = (PosicionJugador - GetActorLocation()).GetSafeNormal();
    }
    else
    {
        DireccionVuelo = GetActorForwardVector();
    }

    // Timer de rastro y vida útil (damos más tiempo de vida si nace muy lejos)
    GetWorldTimerManager().SetTimer(TimerRastrosHandle, this, &AFragmentoErupcion::SoltarRastro, 0.15f, true);

    // Aumentamos el LifeSpan a 10 segundos para que llegue bien desde lejos
    SetLifeSpan(10.0f);
}

void AFragmentoErupcion::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    SetActorLocation(GetActorLocation() + (DireccionVuelo * Velocidad * DeltaTime));
}

void AFragmentoErupcion::SoltarRastro()
{
    // Obtenemos el mundo correctamente dentro de la función
    UWorld* CurrentWorld = GetWorld();
    if (CurrentWorld)
    {
        CurrentWorld->SpawnActor<ARastrosFuego>(ARastrosFuego::StaticClass(), GetActorLocation(), FRotator::ZeroRotator);
    }
}