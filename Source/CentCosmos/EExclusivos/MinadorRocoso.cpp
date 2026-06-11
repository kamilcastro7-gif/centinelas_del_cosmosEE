// Fill out your copyright notice in the Description page of Project Settings.

#include "MinadorRocoso.h"
#include "FragmentoMinador.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AMinadorRocoso::AMinadorRocoso()
{
	NivelExclusivo = 2;
	Vida = 12.0f;
	Velocidad = 400.0f;               // Velocidad de posicionamiento / persecución
	VelocidadEmbestida = 2500.0f;     // ¡SÚPER VELOCIDAD! Un misil indetectable
	DistanciaLargaFrenado = 1000.0f;  // Mantiene la distancia periférica ideal

	EstadoActual = EMinadorEstado::Posicionandose;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PipeMesh(TEXT("StaticMesh'/Game/Assetssss/A6/Meshy_AI_Deep_Space_Asteroid_M_0607153836_texture.Meshy_AI_Deep_Space_Asteroid_M_0607153836_texture'"));

	if (PipeMesh.Succeeded() && EnemigoMesh != nullptr)
	{
		EnemigoMesh->SetStaticMesh(PipeMesh.Object);
	}
	SetActorScale3D(FVector(2.f, 2.f, 2.f));
}

void AMinadorRocoso::BeginPlay()
{
	Super::BeginPlay();
}

void AMinadorRocoso::Tick(float DeltaTime)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn || EstadoActual == EMinadorEstado::Muerto)
	{
		return;
	}

	FVector PosicionActual = GetActorLocation();

	switch (EstadoActual)
	{
	case EMinadorEstado::Posicionandose:
	{
		FVector PosicionJugador = PlayerPawn->GetActorLocation();
		PosicionJugador.Z = PosicionActual.Z; // Bloqueo en el plano horizontal

		float DistanciaAlJugador = FVector::Dist(PosicionActual, PosicionJugador);
		FVector DireccionAlJugador = (PosicionJugador - PosicionActual).GetSafeNormal();
		FVector NuevaPosicion = PosicionActual;

		// LÓGICA DE MANTENER DISTANCIA EXACTA:
		if (DistanciaAlJugador > (DistanciaLargaFrenado + 50.0f))
		{
			// Si está muy lejos, se aproxima hacia el jugador
			NuevaPosicion = PosicionActual + (DireccionAlJugador * Velocidad * DeltaTime);
			SetActorLocation(NuevaPosicion);
		}
		else if (DistanciaAlJugador < (DistanciaLargaFrenado - 50.0f))
		{
			// Si el jugador se le acerca, el Minador retrocede para mantener la distancia larga
			NuevaPosicion = PosicionActual - (DireccionAlJugador * Velocidad * DeltaTime);
			SetActorLocation(NuevaPosicion);
		}
		else
		{
			// Una vez estabilizado en el rango de las 1000 unidades, se frena e inicia los 10 segundos
			EstadoActual = EMinadorEstado::EsperandoTimer;

			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				World->GetTimerManager().SetTimer(TimerCargaHandle, this, &AMinadorRocoso::IniciarEmbestida, 10.0f, false);
			}
		}
		break;
	}

	case EMinadorEstado::EsperandoTimer:
	{
		// SE QUEDA COMPLETAMENTE QUIETO POR 10 SEGUNDOS ESPERANDO LA INICIACIÓN
		break;
	}

	case EMinadorEstado::Embestiendo:
	{
		// EMBESTIDA EN LÍNEA RECTA A MÁXIMA VELOCIDAD
		FVector DireccionEmbestida = (PosicionObjetivoEmbestida - PosicionActual).GetSafeNormal();
		FVector NuevaPosicion = PosicionActual + (DireccionEmbestida * VelocidadEmbestida * DeltaTime);
		NuevaPosicion.Z = PosicionActual.Z;

		SetActorLocation(NuevaPosicion);

		// Al llegar al punto exacto guardado, detona
		if (FVector::DistSquared(PosicionActual, PosicionObjetivoEmbestida) < 900.0f) // Umbral de colisión matemática
		{
			EstallarEnCruz();
		}
		break;
	}
	}
}

void AMinadorRocoso::IniciarEmbestida()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn != nullptr)
	{
		PosicionObjetivoEmbestida = PlayerPawn->GetActorLocation();
		PosicionObjetivoEmbestida.Z = GetActorLocation().Z; // Evita que se entierre en el suelo

		EstadoActual = EMinadorEstado::Embestiendo;
	}
	else
	{
		EstallarEnCruz();
	}
}

void AMinadorRocoso::EstallarEnCruz()
{
	EstadoActual = EMinadorEstado::Muerto;

	UWorld* const World = GetWorld();
	if (World == nullptr) return;

	FVector OrigenEstallido = GetActorLocation();
	FRotator RotacionFija = FRotator::ZeroRotator;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// SISTEMA CARTESIANO DE FRAGMENTOS SÚPER RÁPIDOS
	TArray<FVector> DireccionesCruz;
	DireccionesCruz.Add(FVector(1.0f, 0.0f, 0.0f));   // +X
	DireccionesCruz.Add(FVector(-1.0f, 0.0f, 0.0f));  // -X
	DireccionesCruz.Add(FVector(0.0f, 1.0f, 0.0f));   // +Y
	DireccionesCruz.Add(FVector(0.0f, -1.0f, 0.0f));  // -Y

	for (const FVector& Dir : DireccionesCruz)
	{
		AFragmentoMinador* Fragmento = World->SpawnActor<AFragmentoMinador>(AFragmentoMinador::StaticClass(), OrigenEstallido, RotacionFija, SpawnParams);
		if (Fragmento != nullptr)
		{
			Fragmento->DireccionMovimiento = Dir;
		}
	}

	Destroy(); // Fin del Minador Rocoso
}