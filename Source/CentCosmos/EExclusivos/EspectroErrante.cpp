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

	// 1. CUERPO FANTASMA (Atravesable)
	MallaCuerpo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaCuerpo"));
	RootComponent = MallaCuerpo;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TrimMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Trim.Shape_Trim'"));
	if (TrimMesh.Succeeded()) MallaCuerpo->SetStaticMesh(TrimMesh.Object);
	MallaCuerpo->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 2. NÚCLEO FOCAL
	MallaNucleo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaNucleo"));
	MallaNucleo->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BasicSphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (BasicSphere.Succeeded()) MallaNucleo->SetStaticMesh(BasicSphere.Object);

	MallaNucleo->SetRelativeLocation(FVector(0.0f, 0.0f, 45.0f));
	MallaNucleo->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	VelocidadFlotacion = 220.0f;
	bEsInvulnerable = true;

	// Dirección inicial por defecto
	DireccionErrante = FVector(1.0f, 0.0f, 0.0f);
}

void AEspectroErrante::BeginPlay()
{
	AEExclusivo::BeginPlay();

	CambiarDireccionAleatoria();

	// CAMBIO: Ahora cambia de rumbo cada 0.8 segundos. ¡Se va a mover muchísimo!
	GetWorld()->GetTimerManager().SetTimer(TimerRumboHandle, this, &AEspectroErrante::CambiarDireccionAleatoria, 0.8f, true);

	EntrarFaseInvisible();
}

void AEspectroErrante::CambiarDireccionAleatoria()
{
	// Generamos un rumbo completamente aleatorio en los 360 grados del plano
	float AnguloAleatorio = FMath::FRandRange(0.0f, 360.0f);
	float Radianes = FMath::DegreesToRadians(AnguloAleatorio);

	// Guardamos el vector de avance limpio
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

		// Ignoramos diferencias de altura para el cálculo matemático
		FVector PosActual2D = FVector(PosicionActual.X, PosicionActual.Y, 0.0f);
		FVector PosJugador2D = FVector(PosicionJugador.X, PosicionJugador.Y, 0.0f);

		float Distancia2D = FVector::Dist(PosActual2D, PosJugador2D);

		// Dirección de la mirada directa a la Centinela Apex
		FVector DireccionAlJugador = (PosJugador2D - PosActual2D).GetSafeNormal();

// =========================================================================
// MÁQUINA DE ESTADOS REESTRUCTURADA
// =========================================================================

// ESTADO 1: ALARMA (¡Subimos la distancia objetivo a 650 unidades!)
		if (Distancia2D < 650.0f)
		{
			// Cancelamos cualquier rumbo aleatorio y huimos limpiamente en línea recta
			FVector DireccionHuida = -DireccionAlJugador;

			// Movimiento de escape directo, rápido y sin interferencias
			FVector NuevaPosicion = PosicionActual + (DireccionHuida * (VelocidadFlotacion * 1.4f) * DeltaTime);
			SetActorLocation(NuevaPosicion);
		}
		// ESTADO 2: MERODEO ERRANTE (El jugador está lejos, vagamos fluidamente)
		else
		{
			// Se desplaza de forma continua siguiendo el vector limpio calculado por el Timer
			FVector NuevaPosicion = PosicionActual + (DireccionErrante * VelocidadFlotacion * DeltaTime);
			SetActorLocation(NuevaPosicion);
		}

		// Se mantiene apuntando y mirando de frente al jugador en todo momento
		FRotator NuevaRotacion = DireccionAlJugador.Rotation();
		SetActorRotation(NuevaRotacion);
	}
}


void AEspectroErrante::EntrarFaseInvisible()
{
	bEsInvulnerable = true;

	// FASE SPECTRO: Ocultamos el núcleo y apagamos su colisión (las balas pasan de largo)
	if (MallaNucleo)
	{
		MallaNucleo->SetHiddenInGame(true);
		MallaNucleo->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// El cuerpo se vuelve translúcido
	if (MallaCuerpo) MallaCuerpo->SetScalarParameterValueOnMaterials(TEXT("Opacity"), 0.15f);

	// Espera 7 segundos acechando en las sombras
	GetWorld()->GetTimerManager().SetTimer(TimerFaseExpuestoHandle, this, &AEspectroErrante::EntrarFaseExpuesto, 7.0f, false);
}

void AEspectroErrante::EntrarFaseExpuesto()
{
	bEsInvulnerable = false;

	// FASE EXPUESTO: Activamos el núcleo físicamente para que reciba los impactos
	if (MallaNucleo)
	{
		MallaNucleo->SetHiddenInGame(false);
		MallaNucleo->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		MallaNucleo->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	}

	if (MallaCuerpo) MallaCuerpo->SetScalarParameterValueOnMaterials(TEXT("Opacity"), 1.0f);

	// ATAQUE EMBOSCADA RÁPIDO: Dispara de golpe al jugador al rebelarse
	EjecutarDisparoEspectral();

	// Ventana de 3 segundos exactos donde es vulnerable antes de volver a ocultarse
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