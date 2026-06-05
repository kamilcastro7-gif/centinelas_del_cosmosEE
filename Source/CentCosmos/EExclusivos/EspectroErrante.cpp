// Fill out your copyright notice in the Description page of Project Settings.

#include "EspectroErrante.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "ProyectilEspectro.h"

AEspectroErrante::AEspectroErrante()
{
	PrimaryActorTick.bCanEverTick = true;
	Vida = 13.0f;

	MallaCuerpo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaCuerpo"));
	RootComponent = MallaCuerpo;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TrimMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Trim.Shape_Trim'"));
	if (TrimMesh.Succeeded()) MallaCuerpo->SetStaticMesh(TrimMesh.Object);
	MallaCuerpo->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MallaNucleo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaNucleo"));
	MallaNucleo->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BasicSphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (BasicSphere.Succeeded()) MallaNucleo->SetStaticMesh(BasicSphere.Object);

	MallaNucleo->SetRelativeLocation(FVector(0.0f, 0.0f, 45.0f));
	MallaNucleo->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	VelocidadFlotacion = 220.0f;
	bEsInvulnerable = true;
	DireccionErrante = FVector(1.0f, 0.0f, 0.0f);
}

void AEspectroErrante::BeginPlay()
{
	AEExclusivo::BeginPlay();

	CambiarDireccionAleatoria();
	GetWorld()->GetTimerManager().SetTimer(TimerRumboHandle, this, &AEspectroErrante::CambiarDireccionAleatoria, 0.8f, true);
	EntrarFaseInvisible();
}

void AEspectroErrante::CambiarDireccionAleatoria()
{
	float AnguloAleatorio = FMath::FRandRange(0.0f, 360.0f);
	float Radianes = FMath::DegreesToRadians(AnguloAleatorio);
	DireccionErrante = FVector(FMath::Cos(Radianes), FMath::Sin(Radianes), 0.0f);
}

void AEspectroErrante::Tick(float DeltaTime)
{
	AEExclusivo::Tick(DeltaTime);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		FVector PosicionActual = GetActorLocation();
		FVector PosicionJugador = PlayerPawn->GetActorLocation();

		FVector PosActual2D = FVector(PosicionActual.X, PosicionActual.Y, 0.0f);
		FVector PosJugador2D = FVector(PosicionJugador.X, PosicionJugador.Y, 0.0f);

		float Distancia2D = FVector::Dist(PosActual2D, PosJugador2D);
		FVector DireccionAlJugador = (PosJugador2D - PosActual2D).GetSafeNormal();

		if (Distancia2D < 650.0f)
		{
			FVector DireccionHuida = -DireccionAlJugador;
			FVector NuevaPosicion = PosicionActual + (DireccionHuida * (VelocidadFlotacion * 1.4f) * DeltaTime);
			SetActorLocation(NuevaPosicion);
		}
		else
		{
			FVector NuevaPosicion = PosicionActual + (DireccionErrante * VelocidadFlotacion * DeltaTime);
			SetActorLocation(NuevaPosicion);
		}

		FRotator NuevaRotacion = DireccionAlJugador.Rotation();
		SetActorRotation(NuevaRotacion);
	}
}

void AEspectroErrante::EntrarFaseInvisible()
{
	bEsInvulnerable = true;

	if (MallaNucleo)
	{
		MallaNucleo->SetHiddenInGame(true);
		MallaNucleo->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (MallaCuerpo) MallaCuerpo->SetScalarParameterValueOnMaterials(TEXT("Opacity"), 0.15f);

	GetWorld()->GetTimerManager().SetTimer(TimerFaseExpuestoHandle, this, &AEspectroErrante::EntrarFaseExpuesto, 7.0f, false);
}

void AEspectroErrante::EntrarFaseExpuesto()
{
	bEsInvulnerable = false;

	if (MallaNucleo)
	{
		MallaNucleo->SetHiddenInGame(false);
		MallaNucleo->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		MallaNucleo->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	}

	if (MallaCuerpo) MallaCuerpo->SetScalarParameterValueOnMaterials(TEXT("Opacity"), 1.0f);

	EjecutarDisparoEspectral();
	GetWorld()->GetTimerManager().SetTimer(TimerFaseInvisibleHandle, this, &AEspectroErrante::EntrarFaseInvisible, 3.0f, false);
}

void AEspectroErrante::EjecutarDisparoEspectral()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector UbicacionDisparo = MallaNucleo->GetComponentLocation() + (GetActorForwardVector() * 80.0f);
		FRotator RotacionDisparo = GetActorRotation();

		World->SpawnActor<AProyectilEspectro>(AProyectilEspectro::StaticClass(), UbicacionDisparo, RotacionDisparo, SpawnParams);
	}
}

void AEspectroErrante::RecibirDanoEnemigo(float CantidadDano)
{
	if (!bEsInvulnerable)
	{
		Super::RecibirDanoEnemigo(CantidadDano);
	}
}