// Fill out your copyright notice in the Description page of Project Settings.
#include "Enjambre.h"
#include "Components/StaticMeshComponent.h"
#include "CentCosmosPawn.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "SobrecargaApex.h"
#include "MunicionDispersion.h"
#include "Decorator/EnemBaseComp.h"
#include "Decorator/EnemDecorador.h"
#include "NucleoEter.h"

AEnjambre::AEnjambre()
{
	PrimaryActorTick.bCanEverTick = true;

	MallaEnemigo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaEnemigo"));
	MallaEnemigo->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = MallaEnemigo;

	// Instanciar el decorador
	DecoradorVida = CreateDefaultSubobject<UEnemDecorador>(TEXT("DecoradorVida"));

	VidaActual = 10.0f;
	DanioDeChoque = 1.0f;
	ProbabilidadDrop = 0.3f;
}

void AEnjambre::BeginPlay()
{
	Super::BeginPlay();

	// Inicializar la vida en el decorador
	if (DecoradorVida)
	{
		UEnemBaseComp* Base = NewObject<UEnemBaseComp>(this);
		Base->Inicializar(VidaActual);
		DecoradorVida->Envolver(TScriptInterface<IEnemigo>(Base));
	}
}

void AEnjambre::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnjambre::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
	{
		ACentCosmosPawn* NaveJugador = Cast<ACentCosmosPawn>(OtherActor);
		if (NaveJugador)
		{
			NaveJugador->RecibirDanioNave(DanioDeChoque);
		}
	}
}

void AEnjambre::RecibirDanioEnemigo(float Cantidad)
{
	// Delegar el daño al decorador
	if (DecoradorVida)
	{
		DecoradorVida->RecibirDanio(Cantidad);
		VidaActual = DecoradorVida->GetVida(); // Sincronizamos la variable local
	}
	else
	{
		// Fallback de seguridad
		VidaActual -= Cantidad;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow,
			FString::Printf(TEXT("Enemigo recibio %.1f de dano. Vida restante: %.1f"), Cantidad, VidaActual));
	}

	// Comprobamos si ha muerto
	if (DecoradorVida ? !DecoradorVida->EstaVivo() : VidaActual <= 0.0f)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("ENEMIGO DESTRUIDO"));
		}

		DropPowerUp();
		Destroy();
	}
}

void AEnjambre::DropPowerUp()
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return;

	const float Tirada = FMath::FRand();
	if (Tirada > ProbabilidadDrop) return;

	const int32 Eleccion = FMath::RandRange(0, 2);

	FVector Posicion = GetActorLocation();
	FRotator Rotacion = FRotator::ZeroRotator;
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	switch (Eleccion)
	{
	case 0:
		Mundo->SpawnActor<APowerUpSobrecargaApex>(APowerUpSobrecargaApex::StaticClass(), Posicion, Rotacion, Params);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("[Drop] SobrecargaApex!"));
		break;
	case 1:
		Mundo->SpawnActor<AMunicionDispersion>(AMunicionDispersion::StaticClass(), Posicion, Rotacion, Params);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("[Drop] MunicionDispersion!"));
		break;
	case 2:
		Mundo->SpawnActor<ANucleoEter>(ANucleoEter::StaticClass(), Posicion, Rotacion, Params);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("[Drop] NucleoEter!"));
		break;
	default:
		break;
	}
}