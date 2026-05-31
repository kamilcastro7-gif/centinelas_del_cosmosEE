// Fill out your copyright notice in the Description page of Project Settings.

#include "BombarderoFuego.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h" // Inclusi�n necesaria para buscar al jugador
#include "ProyectilFuego.h"

ABombarderoFuego::ABombarderoFuego()
{
	PrimaryActorTick.bCanEverTick = true;

	BombarderoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombarderoMesh"));
	RootComponent = BombarderoMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WedgeMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Wedge_B.Shape_Wedge_B'"));
	if (WedgeMesh.Succeeded())
	{
		BombarderoMesh->SetStaticMesh(WedgeMesh.Object);
	}

	VelocidadMovimiento = 400.0f;
	RangoBarrido = 300.0f;
	PosicionCentro = FVector::ZeroVector;
	bMoviendoDerecha = true;
}

void ABombarderoFuego::BeginPlay()
{
	AActor::BeginPlay();

	// CORRECCI�N: Modificado para que ataque estrictamente cada 5 segundos
	GetWorld()->GetTimerManager().SetTimer(TimerAtaqueHandle, this, &ABombarderoFuego::EjecutarDisparoFuego, 5.0f, true);
}

void ABombarderoFuego::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	// =========================================================================
	// NUEVA IA: SEGUIMIENTO EN EL EJE Y MANTENIENDO DISTANCIA
	// =========================================================================
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		FVector PosicionActual = GetActorLocation();
		FVector PosicionJugador = PlayerPawn->GetActorLocation();

		// Calculamos la direcci�n hacia donde se mueve el jugador en Y
		float DireccionY = PosicionJugador.Y - PosicionActual.Y;

		// Si el jugador est� lejos en el eje Y, el bombardero se desplaza para alinearse
		if (FMath::Abs(DireccionY) > 10.0f)
		{
			float Signo = (DireccionY > 0.0f) ? 1.0f : -1.0f;
			PosicionActual.Y += Signo * VelocidadMovimiento * DeltaTime;
			SetActorLocation(PosicionActual);
		}
	}
}

void ABombarderoFuego::moverBombardero()
{
	FVector PosicionActual = GetActorLocation();
	float Desplazamiento = bMoviendoDerecha ? VelocidadMovimiento * 0.1f : -VelocidadMovimiento * 0.1f;
	float NuevaY = PosicionActual.Y + Desplazamiento;
	if (NuevaY > PosicionCentro.Y + RangoBarrido)
	{
		NuevaY = PosicionCentro.Y + RangoBarrido;
		bMoviendoDerecha = false;
	}
	else if (NuevaY < PosicionCentro.Y - RangoBarrido)
	{
		NuevaY = PosicionCentro.Y - RangoBarrido;
		bMoviendoDerecha = true;
	}
	PosicionActual.Y = NuevaY;
	SetActorLocation(PosicionActual);
}

void ABombarderoFuego::EjecutarDisparoFuego()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			// 1. Conseguimos las posiciones exactas
			FVector UbicacionBombardero = GetActorLocation();
			FVector PosicionJugador = PlayerPawn->GetActorLocation();

			// 2. CORRECCI�N S�LIDA: Calculamos la direcci�n real hacia el jugador
			FVector DireccionHaciaJugador = (PosicionJugador - UbicacionBombardero).GetSafeNormal();

			// Ajustamos el spawn un poco al frente en direcci�n al jugador para evitar colisiones propias
			FVector UbicacionDisparo = UbicacionBombardero + (DireccionHaciaJugador * 120.0f);

			// Hacemos que el proyectil rote mirando fijamente hacia el jugador
			FRotator RotacionDisparo = DireccionHaciaJugador.Rotation();

			// 3. Spawneamos el cubo de fuego con la direcci�n e inclinaci�n perfectas
			World->SpawnActor<AProyectilFuego>(AProyectilFuego::StaticClass(), UbicacionDisparo, RotacionDisparo, SpawnParams);
		}
	}
}