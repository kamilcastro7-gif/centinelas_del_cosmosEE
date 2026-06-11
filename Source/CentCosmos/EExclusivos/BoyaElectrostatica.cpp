// Fill out your copyright notice in the Description page of Project Settings.

#include "BoyaElectrostatica.h"
#include "ChispaElectrostatica.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
// --- NUEVO INCLUDE PARA LA MATEMÁTICA DE ROTACIÓN ---
#include "Kismet/KismetMathLibrary.h" 

ABoyaElectrostatica::ABoyaElectrostatica()
{
	NivelExclusivo = 3;
	Vida = 20.0f;
	Velocidad = 380.0f;
	RangoDistanciaJugador = 550.0f;
	TiempoParaNuevoDestino = 0.0f;
	bEstaElevandose = false;          // Bandera de parón estático
	TiempoElevacion = 0.0f;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WedgeMesh(TEXT("StaticMesh'/Game/Assetssss/A8/Meshy_AI_Nebula_Vanguard_0607190219_texture.Meshy_AI_Nebula_Vanguard_0607190219_texture'"));

	if (WedgeMesh.Succeeded() && EnemigoMesh != nullptr)
	{
		EnemigoMesh->SetStaticMesh(WedgeMesh.Object);
	}
}

void ABoyaElectrostatica::BeginPlay()
{
	Super::BeginPlay();
	PosicionBaseZ = GetActorLocation();

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().SetTimer(TimerElevacionHandle, this, &ABoyaElectrostatica::IniciarFaseElevacion, 12.0f, true);
	}
}

void ABoyaElectrostatica::Tick(float DeltaTime)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	FVector PosicionActual = GetActorLocation();
	FRotator RotacionAlJugador = UKismetMathLibrary::FindLookAtRotation(PosicionActual, PlayerPawn->GetActorLocation());

	// Mantenemos Pitch y Roll en 0 para que la nave gire solo sobre su eje Z (como un trompo)
	// y no se entierre apuntando hacia el suelo o el techo.
	RotacionAlJugador.Pitch = 0.0f;
	RotacionAlJugador.Roll = 0.0f;

	RotacionAlJugador.Yaw += 180.0f;

	SetActorRotation(RotacionAlJugador);


	// 1. MANEJO DEL PARÓN DE CARGA DE 2 SEGUNDOS
	if (bEstaElevandose)
	{
		TiempoElevacion += DeltaTime;

		static bool bChispasLanzadas = false;
		if (!bChispasLanzadas)
		{
			SpawnChispasAlRededor();
			bChispasLanzadas = true;
		}

		if (TiempoElevacion >= 2.0f)
		{
			bEstaElevandose = false;
			bChispasLanzadas = false;
			TiempoElevacion = 0.0f;
		}

		return; // Bloquea persecución durante la carga
	}

	// 2. MOVIMIENTO DE ACOSO CONSTANTE
	TiempoParaNuevoDestino -= DeltaTime;
	if (TiempoParaNuevoDestino <= 0.0f)
	{
		TiempoParaNuevoDestino = FMath::RandRange(1.0f, 2.0f);
		FVector PosJugador = PlayerPawn->GetActorLocation();

		FVector OffsetAleatorio = FMath::VRand() * RangoDistanciaJugador;
		OffsetAleatorio.Z = 0.0f;

		DestinoErratico = PosJugador + OffsetAleatorio;
	}

	FVector DireccionHorizontal = (DestinoErratico - PosicionActual);
	DireccionHorizontal.Z = 0.0f;

	if (!DireccionHorizontal.IsNearlyZero(10.0f))
	{
		FVector NuevaPosHorizontal = PosicionActual + (DireccionHorizontal.GetSafeNormal() * Velocidad * DeltaTime);
		NuevaPosHorizontal.Z = PosicionBaseZ.Z;
		SetActorLocation(NuevaPosHorizontal);
	}
}

void ABoyaElectrostatica::moverBoya()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	FVector PosicionActual = GetActorLocation();
	TiempoParaNuevoDestino -= 0.1f;
	if (TiempoParaNuevoDestino <= 0.0f)
	{
		TiempoParaNuevoDestino = FMath::FRandRange(0.5f, 1.0f);
		FVector PosJugador = PlayerPawn->GetActorLocation();
		FVector OffsetAleatorio = FMath::VRand() * RangoDistanciaJugador;
		OffsetAleatorio.Z = 0.0f;
		DestinoErratico = PosJugador + OffsetAleatorio;
	}
	FVector Direccion = (DestinoErratico - PosicionActual);
	Direccion.Z = 0.0f;
	if (!Direccion.IsNearlyZero(10.0f))
	{
		FVector NuevaPos = PosicionActual + Direccion.GetSafeNormal() * Velocidad * 0.1f;
		NuevaPos.Z = PosicionBaseZ.Z;
		SetActorLocation(NuevaPos);
	}
}

void ABoyaElectrostatica::IniciarFaseElevacion()
{
	if (!bEstaElevandose)
	{
		TiempoElevacion = 0.0f;
		bEstaElevandose = true;
	}
}

void ABoyaElectrostatica::SpawnChispasAlRededor()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	FVector CentroBoya = GetActorLocation();
	FVector PosicionJugador = PlayerPawn->GetActorLocation();
	float AlturaZDelJugador = PosicionJugador.Z;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// =========================================================================
	// DETALLE EXCLUSIVO: 3 CHISPAS IMPREDECIBLES MUY CERCANAS AL JUGADOR
	// =========================================================================
	for (int32 i = 0; i < 3; i++)
	{
		// Ángulos variados para envolver a la nave del jugador
		float AnguloJugador = FMath::FRandRange(0.0f, 2.0f * 3.14159f);
		// Distancia corta (entre 100 y 160 unidades) para forzar maniobra de escape
		float DistanciaAlJugador = FMath::FRandRange(100.0f, 160.0f);

		FVector OffsetJugador = FVector(
			FMath::Cos(AnguloJugador) * DistanciaAlJugador,
			FMath::Sin(AnguloJugador) * DistanciaAlJugador,
			0.0f
		);

		FVector PosSpawnCercaJugador = FVector(PosicionJugador.X + OffsetJugador.X, PosicionJugador.Y + OffsetJugador.Y, AlturaZDelJugador);
		World->SpawnActor<AChispaElectrostatica>(AChispaElectrostatica::StaticClass(), PosSpawnCercaJugador, FRotator::ZeroRotator, SpawnParams);
	}

	// =========================================================================
	// CHISPAS RESTANTES (2): GENERADAS PERIFÉRICAS CERCA DE LA BOYA
	// =========================================================================
	for (int32 i = 0; i < 2; i++)
	{
		float AnguloBoya = FMath::FRandRange(0.0f, 2.0f * 3.14159f);
		float DistanciaBoya = FMath::FRandRange(150.0f, 300.0f);

		FVector OffsetBoya = FVector(
			FMath::Cos(AnguloBoya) * DistanciaBoya,
			FMath::Sin(AnguloBoya) * DistanciaBoya,
			0.0f
		);

		FVector PosSpawnCercaBoya = FVector(CentroBoya.X + OffsetBoya.X, CentroBoya.Y + OffsetBoya.Y, AlturaZDelJugador);
		World->SpawnActor<AChispaElectrostatica>(AChispaElectrostatica::StaticClass(), PosSpawnCercaBoya, FRotator::ZeroRotator, SpawnParams);
	}
}