// Fill out your copyright notice in the Description page of Project Settings.

#include "BombarderoFuego.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h" 
#include "ProyectilFuego.h"

ABombarderoFuego::ABombarderoFuego()
{
	PrimaryActorTick.bCanEverTick = true;

	NivelExclusivo = 4;
	Vida = 18.0f;
	VelocidadMovimiento = 500.0f; // Ajusta esto si quieres que persiga más rápido en Y

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WedgeMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Wedge_B.Shape_Wedge_B'"));
	if (WedgeMesh.Succeeded() && EnemigoMesh != nullptr)
	{
		EnemigoMesh->SetStaticMesh(WedgeMesh.Object);
	}
}

void ABombarderoFuego::BeginPlay()
{
	AEExclusivo::BeginPlay();

	// Clavamos su posición inicial para no alterar su distancia (X) ni su altura (Z)
	PosicionInicial = GetActorLocation();

	GetWorld()->GetTimerManager().SetTimer(TimerAtaqueHandle, this, &ABombarderoFuego::EjecutarDisparoFuego, 5.0f, true);
}

void ABombarderoFuego::Tick(float DeltaTime)
{
	AEExclusivo::Tick(DeltaTime);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		FVector PosicionJugador = PlayerPawn->GetActorLocation();
		FVector PosicionActual = GetActorLocation();

		// Interpola SUAVEMENTE solo el eje Y hacia donde está el jugador
		float NuevaY = FMath::FInterpTo(PosicionActual.Y, PosicionJugador.Y, DeltaTime, VelocidadMovimiento / 100.f);

		// Movemos al bombardero bloqueando X y Z en su estado original
		SetActorLocation(FVector(PosicionInicial.X, NuevaY, PosicionInicial.Z));
	}
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

			FVector UbicacionBombardero = GetActorLocation();
			FVector PosicionJugador = PlayerPawn->GetActorLocation();

			FVector DireccionHaciaJugador = (PosicionJugador - UbicacionBombardero).GetSafeNormal();
			FVector UbicacionDisparo = UbicacionBombardero + (DireccionHaciaJugador * 120.0f);
			FRotator RotacionDisparo = DireccionHaciaJugador.Rotation();

			World->SpawnActor<AProyectilFuego>(AProyectilFuego::StaticClass(), UbicacionDisparo, RotacionDisparo, SpawnParams);
		}
	}
}