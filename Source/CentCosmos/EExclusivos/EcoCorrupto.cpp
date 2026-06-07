// Fill out your copyright notice in the Description page of Project Settings.

#include "EcoCorrupto.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProyectilCorrupto1.h"
#include "ProyectilCorrupto2.h"

AEcoCorrupto::AEcoCorrupto()
{
	PrimaryActorTick.bCanEverTick = true;

	Vida = 22.0f;
	VelocidadBase = 380.0f;
	bEsInvulnerable = true;
	bEstaHaciendoDash = false;
	TiempoDashRestante = 0.0f;
	DireccionDash = FVector::ZeroVector;

	// --- CARGA SEGURA DE MALLAS ---

	// 1. Malla Pirámide
	MallaPiramide = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPiramide"));
	RootComponent = MallaPiramide;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PiramideAsset(TEXT("StaticMesh'/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO'"));
	if (PiramideAsset.Succeeded())
	{
		MallaPiramide->SetStaticMesh(PiramideAsset.Object);
	}

	// 2. Malla Nave Eco
	MallaNaveEco = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	MallaNaveEco->SetupAttachment(RootComponent);
	MallaNaveEco->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));

	// Aquí deberías colocar la ruta de tu malla de nave real
	static ConstructorHelpers::FObjectFinder<UStaticMesh> NaveAsset(TEXT("/Game/StarterContent/Shapes/Meshy_AI_Nebula_Vanguard_0525182523_generate.Meshy_AI_Nebula_Vanguard_0525182523_generate"));
	if (NaveAsset.Succeeded())
	{
		MallaNaveEco->SetStaticMesh(NaveAsset.Object);
	}

	MallaNaveEco->SetHiddenInGame(true);
}

void AEcoCorrupto::BeginPlay()
{
	AEExclusivo::BeginPlay();

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerTransformacionHandle, this,
			&AEcoCorrupto::TransformarEnNave, 10.0f, false);
	}
}

void AEcoCorrupto::TransformarEnNave()
{
	bEsInvulnerable = false;

	if (MallaPiramide)
		MallaPiramide->SetHiddenInGame(true);

	if (MallaNaveEco)
	{
		MallaNaveEco->SetHiddenInGame(false);
		MallaNaveEco->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MallaNaveEco->SetCollisionProfileName(TEXT("Pawn"));
	}

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerCicloAtaqueHandle, this,
			&AEcoCorrupto::EjecutarAccionCorrupta, 2.5f, true);
	}
}

void AEcoCorrupto::EjecutarAccionCorrupta()
{
	UWorld* World = GetWorld();
	if (!World) return;

	AActor* Jugador = UGameplayStatics::GetPlayerPawn(World, 0);
	if (!Jugador) return;

	FVector Origen = GetActorLocation();
	FRotator Rotacion = UKismetMathLibrary::FindLookAtRotation(Origen, Jugador->GetActorLocation());

	// Desplazamos 150 unidades hacia adelante para que no nazca dentro de la nave
	FVector SpawnLoc = Origen + (Rotacion.Vector() * 150.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	// BORRAMOS EL INSTIGATOR PORQUE TU ENEMIGO ES ACTOR, NO PAWN
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (FMath::RandBool())
	{
		AProyectilCorrupto1* Proy = World->SpawnActor<AProyectilCorrupto1>(AProyectilCorrupto1::StaticClass(), SpawnLoc, Rotacion, SpawnParams);
		if (Proy && Proy->Movement) {
			Proy->Movement->Velocity = Rotacion.Vector() * 1000.0f;
		}
	}
	else
	{
		AProyectilCorrupto2* Proy = World->SpawnActor<AProyectilCorrupto2>(AProyectilCorrupto2::StaticClass(), SpawnLoc, Rotacion, SpawnParams);
		if (Proy && Proy->Movement) {
			Proy->Movement->Velocity = Rotacion.Vector() * 500.0f;
		}
	}

	bEstaHaciendoDash = true;
	TiempoDashRestante = 0.3f;
	DireccionDash = (Jugador->GetActorLocation() - Origen).GetSafeNormal();
}

void AEcoCorrupto::Tick(float DeltaTime)
{
	AEExclusivo::Tick(DeltaTime);

	if (bEsInvulnerable)
	{
		AddActorLocalRotation(FRotator(0.0f, 45.0f * DeltaTime, 0.0f));
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	FVector PosicionActual = GetActorLocation();
	FVector PosicionJugador = PlayerPawn->GetActorLocation();

	FVector PosActual2D = FVector(PosicionActual.X, PosicionActual.Y, 0.0f);
	FVector PosJugador2D = FVector(PosicionJugador.X, PosicionJugador.Y, 0.0f);

	FVector DireccionAlJugador = (PosJugador2D - PosActual2D).GetSafeNormal();
	SetActorRotation(DireccionAlJugador.Rotation());

	if (bEstaHaciendoDash)
	{
		FVector NuevaPosicion = PosicionActual + (DireccionDash * (VelocidadBase * 3.0f) * DeltaTime);
		SetActorLocation(NuevaPosicion);

		TiempoDashRestante -= DeltaTime;
		if (TiempoDashRestante <= 0.0f)
			bEstaHaciendoDash = false;
	}
	else
	{
		FVector NuevaPosicion = PosicionActual + (DireccionAlJugador * VelocidadBase * DeltaTime);
		SetActorLocation(NuevaPosicion);
	}
}

void AEcoCorrupto::RecibirDanoEnemigo(float CantidadDano)
{
	if (!bEsInvulnerable)
	{
		Super::RecibirDanoEnemigo(CantidadDano);
	}
}