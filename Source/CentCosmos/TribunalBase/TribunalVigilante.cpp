// Fill out your copyright notice in the Description page of Project Settings.

#include "TribunalVigilante.h"
#include "RayoVigia.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Enemies/EVastago_Del_infierno.h"
#include "Enemies/EHeraldo_De_La_Ruina.h"

ATribunalVigilante::ATribunalVigilante()
{
	Vida = 120.0f;
	PrimaryActorTick.bCanEverTick = true;
	VelocidadGiro = 60.f;
	RangoDeteccion = 2200.f;
	bEstaActivo = false;

	// Inicializamos nuestras banderas de fase
	bVastagosInvocados = false;
	bHeraldosInvocados = false;
	bJefeDerrotado = false; 

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PyramidAsset(TEXT("StaticMesh'/Game/Assetssss/A10/Meshy_AI_Orbital_Pyramid_of_Li_0607221452_texture.Meshy_AI_Orbital_Pyramid_of_Li_0607221452_texture'"));
	if (PyramidAsset.Succeeded() && MallaTribunal)
	{
		MallaTribunal->SetStaticMesh(PyramidAsset.Object);
	}

	if (MallaTribunal)
	{
		MallaTribunal->SetRelativeScale3D(FVector(4.f, 4.f, 4.f));
	}

	ComponenteRayo = CreateDefaultSubobject<UChildActorComponent>(TEXT("ComponenteRayo"));
	ComponenteRayo->SetupAttachment(RootComponent);
	ComponenteRayo->SetChildActorClass(ARayoVigia::StaticClass());
	ComponenteRayo->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
}

void ATribunalVigilante::BeginPlay()
{
	Super::BeginPlay();
}

void ATribunalVigilante::ActivarJefe()
{
	APawn* NaveJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (NaveJugador)
	{
		float DistanciaAlJugador = FVector::Dist2D(GetActorLocation(), NaveJugador->GetActorLocation());
		if (DistanciaAlJugador <= RangoDeteccion)
		{
			bEstaActivo = true;
		}
	}
}

void ATribunalVigilante::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Vida <= 91.0f && !bVastagosInvocados)
	{
		TSubclassOf<AActor> ClaseAUsar = ClaseVastago ? ClaseVastago : AEVastago_Del_infierno::StaticClass();

		InvocacionEnCirculo(ClaseAUsar, 8, -1500.f);
		bVastagosInvocados = true;

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("¡El Jefe ha invocado a los Vastagos!"));
	}

	// FASE 2: Invocar 2 Heraldos
	if (Vida <= 51.0f && !bHeraldosInvocados)
	{
		TSubclassOf<AActor> ClaseAUsar = ClaseHeraldo ? ClaseHeraldo : AEHeraldo_De_La_Ruina::StaticClass();

		InvocacionEnCirculo(ClaseAUsar, 2, -1500.f);
		bHeraldosInvocados = true;

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("¡El Jefe ha invocado a los Heraldos!"));
	}

	// ======================================================
	// FASE FINAL: MUERTE DEL JEFE Y LIMPIEZA DE ARENA
	// ======================================================
	if (Vida <= 0.0f && !bJefeDerrotado)
	{
		bJefeDerrotado = true;

		// 1. Buscamos y destruimos a todos los Vástagos vivos
		TArray<AActor*> VastagosVivos;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEVastago_Del_infierno::StaticClass(), VastagosVivos);
		for (AActor* Enemigo : VastagosVivos)
		{
			if (Enemigo) Enemigo->Destroy();
		}

		// 2. Buscamos y destruimos a todos los Heraldos vivos
		TArray<AActor*> HeraldosVivos;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEHeraldo_De_La_Ruina::StaticClass(), HeraldosVivos);
		for (AActor* Enemigo : HeraldosVivos)
		{
			if (Enemigo) Enemigo->Destroy();
		}

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("¡Jefe Derrotado! Limpiando súbditos..."));
	}
	// ======================================================

	APawn* NaveJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (NaveJugador)
	{
		float DistanciaAlJugador = FVector::Dist2D(GetActorLocation(), NaveJugador->GetActorLocation());

		if (DistanciaAlJugador <= RangoDeteccion)
		{
			bEstaActivo = true;
		}
		else
		{
			bEstaActivo = false;
		}
	}

	if (bEstaActivo)
	{
		FRotator NuevaRotacion = GetActorRotation();
		NuevaRotacion.Yaw += VelocidadGiro * DeltaTime;
		SetActorRotation(NuevaRotacion);

		if (ComponenteRayo)
		{
			ARayoVigia* ElRayo = Cast<ARayoVigia>(ComponenteRayo->GetChildActor());
			if (ElRayo)
			{
				ElRayo->SetActorHiddenInGame(false);
				ElRayo->SetActorEnableCollision(true);
				ElRayo->ActualizarLongitudRayo();
			}
		}
	}
	else
	{
		if (ComponenteRayo)
		{
			ARayoVigia* ElRayo = Cast<ARayoVigia>(ComponenteRayo->GetChildActor());
			if (ElRayo)
			{
				ElRayo->SetActorHiddenInGame(true);
				ElRayo->SetActorEnableCollision(false);

				if (ElRayo->MallaRayo)
				{
					ElRayo->MallaRayo->SetHiddenInGame(true);
				}
			}
		}
	}
}

// ============================================================
// LÓGICA DE INVOCACIÓN MATEMÁTICA CORREGIDA (COLISIONES SEGURAS)
// ============================================================
void ATribunalVigilante::InvocacionEnCirculo(TSubclassOf<AActor> ClaseEnemigo, int32 Cantidad, float Radio)
{
	if (!ClaseEnemigo || Cantidad <= 0) return;

	UWorld* Mundo = GetWorld();
	if (!Mundo) return;

	float PasoAngular = 360.0f / Cantidad;

	FActorSpawnParameters Params;
	// Forzamos la aparición aunque colisionen un poco, Unreal se encarga de separarlos
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int32 i = 0; i < Cantidad; i++)
	{
		float AnguloEnRadianes = FMath::DegreesToRadians(i * PasoAngular);

		// CORRECCIÓN CLAVE: Agregamos +150.f en el eje Z (arriba) para que no spawneen atorados en el suelo
		FVector PosicionOffset(FMath::Cos(AnguloEnRadianes) * Radio, FMath::Sin(AnguloEnRadianes) * Radio, 0.f);
		FVector PosicionSpawn = GetActorLocation() + PosicionOffset;

		// Hacemos que los enemigos miren hacia el jugador (o hacia afuera)
		FRotator RotacionSpawn = PosicionOffset.Rotation();

		Mundo->SpawnActor<AActor>(ClaseEnemigo, PosicionSpawn, RotacionSpawn, Params);
	}
}

void ATribunalVigilante::Destroyed()
{
	// 1. Buscamos y destruimos a todos los Vástagos vivos
	TArray<AActor*> VastagosVivos;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEVastago_Del_infierno::StaticClass(), VastagosVivos);
	for (AActor* Enemigo : VastagosVivos)
	{
		if (Enemigo) Enemigo->Destroy();
	}

	// 2. Buscamos y destruimos a todos los Heraldos vivos
	TArray<AActor*> HeraldosVivos;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEHeraldo_De_La_Ruina::StaticClass(), HeraldosVivos);
	for (AActor* Enemigo : HeraldosVivos)
	{
		if (Enemigo) Enemigo->Destroy();
	}

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("¡Jefe Destruido! Súbditos eliminados de la arena."));

	// Es obligatorio llamar al Super para que Unreal termine de borrar al jefe de la memoria
	Super::Destroyed();
}