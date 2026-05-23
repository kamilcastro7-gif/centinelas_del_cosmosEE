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

	VelocidadBase = 380.0f;
	bEsInvulnerable = true;
	bEstaHaciendoDash = false;
	TiempoDashRestante = 0.0f;
	DireccionDash = FVector::ZeroVector;

	MallaPiramide = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPiramide"));
	RootComponent = MallaPiramide;

	MallaNaveEco = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaNaveEco"));
	MallaNaveEco->SetupAttachment(RootComponent);
	MallaNaveEco->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
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
			&AEcoCorrupto::EjecutarAccionCorrupta, 1.5f, true);
	}
}

void AEcoCorrupto::EjecutarAccionCorrupta()
{
	UWorld* World = GetWorld();
	if (!World) return;

	FVector SpawnLoc = GetActorLocation() + (GetActorForwardVector() * 250.0f);
	AActor* Jugador = UGameplayStatics::GetPlayerPawn(World, 0);
	FRotator Rotacion = (Jugador) ? UKismetMathLibrary::FindLookAtRotation(SpawnLoc, Jugador->GetActorLocation()) : GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (FMath::RandBool())
	{
		AProyectilCorrupto1* Proy = World->SpawnActor<AProyectilCorrupto1>(AProyectilCorrupto1::StaticClass(), SpawnLoc, Rotacion, SpawnParams);
		if (Proy && Proy->Movement) {
			Proy->Movement->Velocity = Rotacion.Vector() * 1000.0f;
			Proy->Movement->Activate(true);
		}
	}
	else
	{
		AProyectilCorrupto2* Proy = World->SpawnActor<AProyectilCorrupto2>(AProyectilCorrupto2::StaticClass(), SpawnLoc, Rotacion, SpawnParams);
		if (Proy && Proy->Movement) {
			Proy->Movement->Velocity = Rotacion.Vector() * 500.0f;
			Proy->Movement->Activate(true);
		}
	}

	bEstaHaciendoDash = true;
	TiempoDashRestante = 0.3f;
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