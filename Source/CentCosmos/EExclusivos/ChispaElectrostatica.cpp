// Fill out your copyright notice in the Description page of Project Settings.

#include "ChispaElectrostatica.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "../Player/CentCosmosPawn.h" 
// --- NUEVOS INCLUDES PARA NIAGARA ---
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AChispaElectrostatica::AChispaElectrostatica()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. HITBOX INVISIBLE: Esfera base
	ChispaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChispaMesh"));
	RootComponent = ChispaMesh;

	ChispaMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ChispaMesh->SetCollisionProfileName(TEXT("OverlapAll"));
	ChispaMesh->SetGenerateOverlapEvents(true); // Forzamos colisiones aunque sea invisible

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (MeshAsset.Succeeded())
	{
		ChispaMesh->SetStaticMesh(MeshAsset.Object);
	}

	// Ocultamos la malla visualmente
	ChispaMesh->SetHiddenInGame(true);
	SetActorScale3D(FVector(1.2f, 1.2f, 1.2f));

	// 2. EFECTO VISUAL NIAGARA
	EfectoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoNiagara"));
	EfectoNiagara->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Hit_2.NS_Hit_2'"));
	if (NiagaraAsset.Succeeded())
	{
		EfectoNiagara->SetAsset(NiagaraAsset.Object);
	}

	AmplitudFlotacion = 75.0f;
	VelocidadFlotacion = 7.0f;
	TiempoFlotacion = 0.0f;

	FireRateOriginal = 0.0f;
}

void AChispaElectrostatica::BeginPlay()
{
	Super::BeginPlay(); // Cambiado a Super::
	PosicionInicial = GetActorLocation();
	TiempoFlotacion = FMath::RandRange(0.0f, 10.0f);
	SetLifeSpan(7.0f);
}

void AChispaElectrostatica::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // Cambiado a Super::

	TiempoFlotacion += DeltaTime * VelocidadFlotacion;
	FVector PosicionActual = GetActorLocation();
	PosicionActual.Z = PosicionInicial.Z + (FMath::Sin(TiempoFlotacion) * AmplitudFlotacion);
	SetActorLocation(PosicionActual);
}

void AChispaElectrostatica::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor); // Cambiado a Super::

	if (OtherActor != nullptr && OtherActor != this && OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		ACentCosmosPawn* NaveJugador = Cast<ACentCosmosPawn>(OtherActor);
		if (NaveJugador != nullptr)
		{
			// 1. EFECTO BALAS NORMALES (Aumenta el delay entre ráfagas)
			FFloatProperty* FireRateProp = CastField<FFloatProperty>(NaveJugador->GetClass()->FindPropertyByName(TEXT("FireRate")));
			if (!FireRateProp) FireRateProp = CastField<FFloatProperty>(NaveJugador->GetClass()->FindPropertyByName(TEXT("FireDelay")));

			if (FireRateProp)
			{
				FireRateOriginal = FireRateProp->GetPropertyValue_InContainer(NaveJugador);
				FireRateProp->SetPropertyValue_InContainer(NaveJugador, FireRateOriginal * 1.5f);
			}

			// =========================================================================
			// ACTIVACIÓN DEL FLAG PARA BOOMERANG Y CARGA
			// =========================================================================
			NaveJugador->bRalentizadoPorChispa = true;

			// Al ocultar el Actor, también se ocultará el sistema Niagara. ¡Perfecto para cuando se consume!
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);

			// LIMITADOR SAGRADO DE 5 SEGUNDOS
			FTimerHandle TimerRestaurarHandle;
			FTimerDelegate DelegateRestaurar;

			DelegateRestaurar.BindUObject(this, &AChispaElectrostatica::RestaurarAtributosJugador, OtherActor);
			GetWorld()->GetTimerManager().SetTimer(TimerRestaurarHandle, DelegateRestaurar, 5.0f, false);
		}
	}
}

void AChispaElectrostatica::RestaurarAtributosJugador(AActor* JugadorCasteado)
{
	ACentCosmosPawn* NaveJugador = Cast<ACentCosmosPawn>(JugadorCasteado);
	if (NaveJugador != nullptr)
	{
		// RESTAURACIÓN: Balas normales vuelven a su cadencia original
		FFloatProperty* FireRateProp = CastField<FFloatProperty>(NaveJugador->GetClass()->FindPropertyByName(TEXT("FireRate")));
		if (!FireRateProp) FireRateProp = CastField<FFloatProperty>(NaveJugador->GetClass()->FindPropertyByName(TEXT("FireDelay")));

		if (FireRateProp && FireRateOriginal > 0.0f)
		{
			FireRateProp->SetPropertyValue_InContainer(NaveJugador, FireRateOriginal);
		}

		// =========================================================================
		// DESACTIVACIÓN DEL FLAG AL PASAR LOS 5 SEGUNDOS
		// =========================================================================
		NaveJugador->bRalentizadoPorChispa = false;
	}

	Destroy();
}
