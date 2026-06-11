// Fill out your copyright notice in the Description page of Project Settings.

#include "Ambientacion.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

AAmbientacion::AAmbientacion()
{
	PrimaryActorTick.bCanEverTick = false;

	// Creamos el componente vacío.
	MallaObstaculo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaObstaculo"));
	if (MallaObstaculo)
	{
		RootComponent = MallaObstaculo;
	}
}

void AAmbientacion::BeginPlay()
{
	Super::BeginPlay();

	EvitarColisionConJugador();
}

void AAmbientacion::InicializarObstaculo()
{
}

void AAmbientacion::EvitarColisionConJugador()
{
	// Obtenemos al jugador principal
	APawn* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (Jugador)
	{
		float Distancia = FVector::Dist(GetActorLocation(), Jugador->GetActorLocation());

		// Si la distancia es menor a 600 unidades, consideramos que están chocando o muy incómodos
		// (Puedes subir este número a 800 o 1000 si tus obstáculos son gigantes)
		if (Distancia < 600.0f)
		{
			FVector NuevaPosicion = GetActorLocation();

			// Generamos un empujón aleatorio entre 1000 y 2000 unidades en los ejes X e Y
			// El FMath::RandBool() decide si el salto es hacia la izquierda/derecha o arriba/abajo
			float OffsetX = FMath::RandRange(1000.f, 2000.f) * (FMath::RandBool() ? 1 : -1);
			float OffsetY = FMath::RandRange(1000.f, 2000.f) * (FMath::RandBool() ? 1 : -1);

			NuevaPosicion.X += OffsetX;
			NuevaPosicion.Y += OffsetY;

			// Teletransportamos el obstáculo a la nueva posición segura
			SetActorLocation(NuevaPosicion, false, nullptr, ETeleportType::TeleportPhysics);

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Obstaculo reubicado automaticamente"));
			}
		}
	}
}

