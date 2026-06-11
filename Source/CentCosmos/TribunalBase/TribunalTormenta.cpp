// Fill out your copyright notice in the Description page of Project Settings.

#include "TribunalTormenta.h"
#include "ProyectilPlasma.h"
#include "ProyectilRayo.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
// --- INCLUDES DE ENEMIGOS EXACTOS ---
#include "Enemies/EVastago_Del_infierno.h"
#include "Enemies/EHeraldo_De_La_Ruina.h"
#include "BoyaElectrostatica.h" // NUEVO: Tu clase exacta de la Boya

ATribunalTormenta::ATribunalTormenta()
{
	PrimaryActorTick.bCanEverTick = true;
	bEstaActivo = false;

	MiMalla = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MiMallaTormenta"));
	RootComponent = MiMalla;
	MiMalla->SetMobility(EComponentMobility::Movable);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Assetssss/A13/Meshy_AI_Crimson_Vortex_Gate_0607221607_texture.Meshy_AI_Crimson_Vortex_Gate_0607221607_texture'"));
	if (MeshAsset.Succeeded())
	{
		MiMalla->SetStaticMesh(MeshAsset.Object);
		MiMalla->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
		MiMalla->SetCollisionProfileName(TEXT("BlockAll"));
	}

	ClasePlasma = AProyectilPlasma::StaticClass();
	ClaseRayo = AProyectilRayo::StaticClass();

	Vida = 150.0f;
	VelocidadDesplazamiento = 250.f;
	RadioMaximoMovimiento = 500.f;
	TiempoEsperaEnPunto = 1.5f;
	RangoDeteccionManual = 2200.f;
	bPuedeMoverse = true;

	CadenciaRayo = 2.0f;
	CadenciaPlasma = 4.0f;
	CronometroRayo = 0.f;
	CronometroPlasma = 0.f;

	// Inicializamos nuestras banderas
	bBoyas120Invocadas = false;
	bHeraldos80Invocados = false;
	bVastagos40Invocados = false;
}

void ATribunalTormenta::BeginPlay()
{
	Super::BeginPlay();
	PosicionInicialAnclaje = GetActorLocation();
	PuntoDestinoActual = PosicionInicialAnclaje;
}

// ============================================================
// TESTAMENTO DEL JEFE: Limpieza de la arena al morir
// ============================================================
void ATribunalTormenta::Destroyed()
{
	// 1. Limpiamos Vastagos
	TArray<AActor*> VastagosVivos;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEVastago_Del_infierno::StaticClass(), VastagosVivos);
	for (AActor* Enemigo : VastagosVivos) { if (Enemigo) Enemigo->Destroy(); }

	// 2. Limpiamos Heraldos
	TArray<AActor*> HeraldosVivos;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEHeraldo_De_La_Ruina::StaticClass(), HeraldosVivos);
	for (AActor* Enemigo : HeraldosVivos) { if (Enemigo) Enemigo->Destroy(); }

	// 3. Limpiamos Boyas (Usando tu clase exacta ahora)
	TArray<AActor*> BoyasVivas;
	TSubclassOf<AActor> ClaseParaLimpiar = ClaseBoya ? ClaseBoya : ABoyaElectrostatica::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClaseParaLimpiar, BoyasVivas);
	for (AActor* Enemigo : BoyasVivas) { if (Enemigo) Enemigo->Destroy(); }

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("¡Jefe Tormenta Destruido! Arena limpia."));

	Super::Destroyed();
}

void ATribunalTormenta::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// === BLINDAJE: Cortar si el jefe está muriendo ===
	if (!IsValid(this) || IsActorBeingDestroyed()) return;

	// ======================================================
	// VIGILANCIA DE FASES DE INVOCACIÓN
	// ======================================================

	// FASE 1: 120 de vida -> Invoca 2 Boyas Electrostáticas
	if (Vida <= 120.0f && !bBoyas120Invocadas)
	{
		// AHORA SÍ: Usamos ABoyaElectrostatica de forma directa y a prueba de fallos
		TSubclassOf<AActor> ClaseAUsar = ClaseBoya ? ClaseBoya : ABoyaElectrostatica::StaticClass();
		InvocacionEnCirculo(ClaseAUsar, 2, 700.f);
		bBoyas120Invocadas = true;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("¡Tormenta invoca Boyas Electrostaticas!"));
	}

	// FASE 2: 80 de vida -> Invoca 2 Heraldos
	if (Vida <= 80.0f && !bHeraldos80Invocados)
	{
		TSubclassOf<AActor> ClaseAUsar = ClaseHeraldo ? ClaseHeraldo : AEHeraldo_De_La_Ruina::StaticClass();
		InvocacionEnCirculo(ClaseAUsar, 2, 800.f);
		bHeraldos80Invocados = true;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("¡Tormenta invoca Heraldos!"));
	}

	// FASE 3: 40 de vida -> Invoca 8 Vástagos
	if (Vida <= 40.0f && !bVastagos40Invocados)
	{
		TSubclassOf<AActor> ClaseAUsar = ClaseVastago ? ClaseVastago : AEVastago_Del_infierno::StaticClass();
		InvocacionEnCirculo(ClaseAUsar, 8, 900.f);
		bVastagos40Invocados = true;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("¡Tormenta invoca un enjambre de Vástagos!"));
	}
	// ======================================================

	APawn* NaveJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (NaveJugador)
	{
		float DistanciaAlJugador = FVector::Dist2D(GetActorLocation(), NaveJugador->GetActorLocation());

		if (DistanciaAlJugador <= RangoDeteccionManual)
		{
			if (!bEstaActivo)
			{
				ActivarJefe();
			}
		}
		else
		{
			if (bEstaActivo)
			{
				bEstaActivo = false;
				bPuedeMoverse = false;
				PuntoDestinoActual = PosicionInicialAnclaje;
				GetWorld()->GetTimerManager().ClearTimer(TimerEsperaMovimiento);
			}
		}
	}

	if (bEstaActivo)
	{
		CronometroRayo += DeltaTime;
		CronometroPlasma += DeltaTime;

		if (CronometroRayo >= CadenciaRayo)
		{
			DispararRayo();
			CronometroRayo = 0.f;
		}

		if (CronometroPlasma >= CadenciaPlasma)
		{
			SpawnPlasma();
			CronometroPlasma = 0.f;
		}

		FVector PosicionActual = GetActorLocation();

		if (bPuedeMoverse && !PosicionActual.Equals(PuntoDestinoActual, 10.f))
		{
			FVector NuevaPosicion = FMath::VInterpTo(PosicionActual, PuntoDestinoActual, DeltaTime, VelocidadDesplazamiento / 100.f);
			NuevaPosicion.Z = PosicionInicialAnclaje.Z;
			SetActorLocation(NuevaPosicion);
		}
		else if (bPuedeMoverse)
		{
			bPuedeMoverse = false;
			GetWorld()->GetTimerManager().SetTimer(TimerEsperaMovimiento, this, &ATribunalTormenta::CalcularNuevoPuntoAleatorio, TiempoEsperaEnPunto, false);
		}
	}
	else
	{
		FVector PosicionActual = GetActorLocation();
		if (!PosicionActual.Equals(PosicionInicialAnclaje, 5.f))
		{
			FVector RegresoSuave = FMath::VInterpTo(PosicionActual, PosicionInicialAnclaje, DeltaTime, 2.0f);
			SetActorLocation(RegresoSuave);
		}
	}
}

// ============================================================
// LÓGICA DE INVOCACIÓN MATEMÁTICA CORREGIDA (COLISIONES SEGURAS)
// ============================================================
void ATribunalTormenta::InvocacionEnCirculo(TSubclassOf<AActor> ClaseEnemigo, int32 Cantidad, float Radio)
{
	if (!ClaseEnemigo || Cantidad <= 0) return;

	UWorld* Mundo = GetWorld();
	if (!Mundo) return;

	float PasoAngular = 360.0f / Cantidad;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int32 i = 0; i < Cantidad; i++)
	{
		float AnguloEnRadianes = FMath::DegreesToRadians(i * PasoAngular);

		FVector PosicionOffset(FMath::Cos(AnguloEnRadianes) * Radio, FMath::Sin(AnguloEnRadianes) * Radio, 0.f);
		FVector PosicionSpawn = GetActorLocation() + PosicionOffset;

		FRotator RotacionSpawn = PosicionOffset.Rotation();

		Mundo->SpawnActor<AActor>(ClaseEnemigo, PosicionSpawn, RotacionSpawn, Params);
	}
}

void ATribunalTormenta::ActivarJefe()
{
	bEstaActivo = true;
	bPuedeMoverse = true;
	PuntoDestinoActual = PosicionInicialAnclaje;

	CronometroRayo = 0.f;
	CronometroPlasma = 0.f;
}

void ATribunalTormenta::CalcularNuevoPuntoAleatorio()
{
	if (!bEstaActivo) return;

	float DesfaseX = FMath::FRandRange(-RadioMaximoMovimiento, RadioMaximoMovimiento);
	float DesfaseY = FMath::FRandRange(-RadioMaximoMovimiento, RadioMaximoMovimiento);

	PuntoDestinoActual = PosicionInicialAnclaje + FVector(DesfaseX, DesfaseY, 0.f);
	bPuedeMoverse = true;
}

void ATribunalTormenta::DispararRayo()
{
	APawn* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Jugador && ClaseRayo)
	{
		FVector MiPosicion = GetActorLocation();

		FVector Direccion = (Jugador->GetActorLocation() - MiPosicion);
		Direccion.Z = 0.f;
		Direccion.Normalize();

		FVector PuntoSpawnExterior = MiPosicion + (Direccion * 350.f);
		PuntoSpawnExterior.Z = Jugador->GetActorLocation().Z;

		FRotator RotacionPlana = Direccion.Rotation();
		RotacionPlana.Pitch = 0.f;
		RotacionPlana.Roll = 0.f;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AProyectilRayo* Proyectil = GetWorld()->SpawnActor<AProyectilRayo>(ClaseRayo, PuntoSpawnExterior, RotacionPlana, SpawnParams);
		if (Proyectil)
		{
			Proyectil->DireccionVuelo = Direccion;
		}
	}
}

void ATribunalTormenta::SpawnPlasma()
{
	APawn* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Jugador && ClasePlasma)
	{
		FVector MiPosicion = GetActorLocation();

		FVector DireccionBase = (Jugador->GetActorLocation() - MiPosicion);
		DireccionBase.Z = 0.f;
		DireccionBase.Normalize();

		FRotator BaseRotation = DireccionBase.Rotation();
		BaseRotation.Pitch = 0.f;
		BaseRotation.Roll = 0.f;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		float Angulos[3] = { -20.f, 0.f, 20.f };
		for (int32 i = 0; i < 3; i++)
		{
			FRotator RotacionDisparo = BaseRotation;
			RotacionDisparo.Yaw += Angulos[i];

			FVector DireccionProyectil = RotacionDisparo.Vector();
			DireccionProyectil.Z = 0.f;
			DireccionProyectil.Normalize();

			FVector PuntoSpawnExterior = MiPosicion + (DireccionProyectil * 350.f);
			PuntoSpawnExterior.Z = Jugador->GetActorLocation().Z;

			AProyectilPlasma* Proyectil = GetWorld()->SpawnActor<AProyectilPlasma>(ClasePlasma, PuntoSpawnExterior, RotacionDisparo, SpawnParams);
			if (Proyectil)
			{
				Proyectil->DireccionVuelo = DireccionProyectil;
			}
		}
	}
}
