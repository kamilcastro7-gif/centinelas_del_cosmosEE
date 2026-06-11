// Fill out your copyright notice in the Description page of Project Settings.

#include "BombarderoFuego.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h" // NUEVO: Necesario para el SkeletalMesh
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h" 
#include "ProyectilFuego.h"

ABombarderoFuego::ABombarderoFuego()
{
	PrimaryActorTick.bCanEverTick = true;

	NivelExclusivo = 4;
	Vida = 18.0f;
	VelocidadMovimiento = 500.0f; // Ajusta esto si quieres que persiga más rápido en Y

	// 1. EL TRUCO DE LA HITBOX: Cargamos la figura base (Wedge)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WedgeMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Wedge_B.Shape_Wedge_B'"));
	if (WedgeMesh.Succeeded() && EnemigoMesh != nullptr)
	{
		EnemigoMesh->SetStaticMesh(WedgeMesh.Object);

		// Lo volvemos invisible en el juego, pero mantenemos su solidez para recibir daño
		EnemigoMesh->SetHiddenInGame(true);
		EnemigoMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		EnemigoMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		EnemigoMesh->SetGenerateOverlapEvents(true);
	}

	// 2. MALLA DEL BOMBARDERO: Creamos el dron solo como componente visual
	MallaBombardero = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MallaBombardero"));
	if (RootComponent) {
		MallaBombardero->SetupAttachment(RootComponent); // Se pega a la hitbox invisible
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DroneAsset(TEXT("SkeletalMesh'/Game/Scifi_Survey_Drones/Drone_03/Meshes/SK_Sci-fi_Survey_Drone_03.SK_Sci-fi_Survey_Drone_03'"));
	if (DroneAsset.Succeeded())
	{
		MallaBombardero->SetSkeletalMesh(DroneAsset.Object);
	}

	// --- ESCALA DEL ASSET ---
	// Mismo tamaño que los demás para mantener coherencia
	MallaBombardero->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));

	// Apagamos la colisión visual para que la figura Wedge haga todos los cálculos matemáticos
	MallaBombardero->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
			SpawnParams.Instigator = GetInstigator(); // Seguridad añadida para el disparo

			FVector UbicacionBombardero = GetActorLocation();
			FVector PosicionJugador = PlayerPawn->GetActorLocation();

			FVector DireccionHaciaJugador = (PosicionJugador - UbicacionBombardero).GetSafeNormal();

			// Aumentamos ligeramente a 150.f para evitar que el proyectil choque con la hitbox
			FVector UbicacionDisparo = UbicacionBombardero + (DireccionHaciaJugador * 150.0f);
			FRotator RotacionDisparo = DireccionHaciaJugador.Rotation();

			World->SpawnActor<AProyectilFuego>(AProyectilFuego::StaticClass(), UbicacionDisparo, RotacionDisparo, SpawnParams);
		}
	}
}