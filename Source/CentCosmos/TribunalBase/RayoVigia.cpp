// Fill out your copyright notice in the Description page of Project Settings.

#include "RayoVigia.h"
#include "DrawDebugHelpers.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/CentCosmosPawn.h"
#include "TimerManager.h" 
#include "Engine/World.h" 

ARayoVigia::ARayoVigia()
{
	PrimaryActorTick.bCanEverTick = false;
	LongitudMaxima = 500.f;

	bPuedeHacerDanio = true;

	USceneComponent* EscenaRaiz = CreateDefaultSubobject<USceneComponent>(TEXT("EscenaRaiz"));
	RootComponent = EscenaRaiz;

	MallaRayo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaRayo"));
	MallaRayo->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (CubeAsset.Succeeded())
	{
		MallaRayo->SetStaticMesh(CubeAsset.Object);
	}

	MallaRayo->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MallaRayo->SetCollisionResponseToAllChannels(ECR_Ignore);

	MallaRayo->SetHiddenInGame(false);
	SetActorHiddenInGame(false);
}

void ARayoVigia::BeginPlay()
{
	Super::BeginPlay();
}

void ARayoVigia::ActualizarLongitudRayo()
{
	if (!MallaRayo) return;

	MallaRayo->SetHiddenInGame(false);

	FVector OriginLocation = MallaRayo->GetComponentLocation();
	FVector DireccionDisparo = MallaRayo->GetRightVector();

	FVector StartLocation = OriginLocation + (DireccionDisparo * 150.f);
	FVector EndLocation = StartLocation + (DireccionDisparo * LongitudMaxima);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;

	CollisionParams.AddIgnoredActor(this);
	if (GetOwner())
	{
		CollisionParams.AddIgnoredActor(GetOwner());
		TArray<AActor*> Hijos;
		GetOwner()->GetAttachedActors(Hijos);
		CollisionParams.AddIgnoredActors(Hijos);
	}

	float LongitudActual = LongitudMaxima;

	// === EL TRUCO: Un rayo "grueso" de 50 de radio para no fallar el tiro ===
	FCollisionShape FormaRayo = FCollisionShape::MakeSphere(50.0f);

	// === BUSCAR SOLO NAVES (Ignoramos paredes y escudos irrelevantes) ===
	FCollisionObjectQueryParams ObjectParams(ECollisionChannel::ECC_Pawn);

	// Usamos SweepSingleByObjectType en lugar de LineTrace
	if (GetWorld()->SweepSingleByObjectType(HitResult, StartLocation, EndLocation, FQuat::Identity, ObjectParams, FormaRayo, CollisionParams))
	{
		LongitudActual = HitResult.Distance + 150.f;

		// CHIVATO: Esto imprimirá en amarillo qué está tocando el rayo
		if (GEngine && HitResult.GetActor())
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, FString::Printf(TEXT("Rayo Vigia toco a: %s"), *HitResult.GetActor()->GetName()));
		}

		// Si lo que tocó fue tu nave, le restamos la vida
		ACentCosmosPawn* NaveImpactada = Cast<ACentCosmosPawn>(HitResult.GetActor());
		if (NaveImpactada)
		{
			if (bPuedeHacerDanio)
			{
				NaveImpactada->RecibirDanioNave(15.0f);
				bPuedeHacerDanio = false;
				GetWorld()->GetTimerManager().SetTimer(TimerCooldownRayo, this, &ARayoVigia::ResetearDanioRayo, 2.0f, false);
			}
		}
	}

	float FactorEscalaY = LongitudActual / 100.f;

	MallaRayo->SetRelativeScale3D(FVector(0.15f, FactorEscalaY, 0.15f));
	MallaRayo->SetRelativeLocation(FVector(0.f, LongitudActual * 0.5f, 0.f));
}

void ARayoVigia::ResetearDanioRayo()
{
	bPuedeHacerDanio = true;
}

