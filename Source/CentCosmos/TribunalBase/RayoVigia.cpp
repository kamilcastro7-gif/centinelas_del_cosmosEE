// Fill out your copyright notice in the Description page of Project Settings.

#include "RayoVigia.h"
#include "DrawDebugHelpers.h" 
#include "UObject/ConstructorHelpers.h"
#include "Player/CentCosmosPawn.h" 
#include "TimerManager.h" 
#include "Engine/World.h" 
#include "Kismet/GameplayStatics.h" 
#include "NiagaraComponent.h"
#include "NiagaraSystem.h" 

ARayoVigia::ARayoVigia()
{
	PrimaryActorTick.bCanEverTick = true;
	LongitudMaxima = 5000.f;
	bPuedeHacerDanio = true;

	USceneComponent* EscenaRaiz = CreateDefaultSubobject<USceneComponent>(TEXT("EscenaRaiz"));
	RootComponent = EscenaRaiz;

	MallaRayo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaRayo"));
	MallaRayo->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (CubeAsset.Succeeded()) MallaRayo->SetStaticMesh(CubeAsset.Object);

	MallaRayo->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MallaRayo->SetCollisionResponseToAllChannels(ECR_Ignore);
	MallaRayo->SetHiddenInGame(true);

	EfectoRayoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoRayoNiagara"));
	EfectoRayoNiagara->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> RayoAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Ray_4.NS_Ray_4'"));
	if (RayoAsset.Succeeded()) EfectoRayoNiagara->SetAsset(RayoAsset.Object);
}

void ARayoVigia::BeginPlay()
{
	Super::BeginPlay();
}

void ARayoVigia::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACentCosmosPawn* Jugador = Cast<ACentCosmosPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Jugador)
	{
		float Distancia = FVector::Dist(GetActorLocation(), Jugador->GetActorLocation());

		if (Distancia <= DistanciaActivacion)
		{
			EfectoRayoNiagara->SetVisibility(true);
			if (!EfectoRayoNiagara->IsActive())
			{
				EfectoRayoNiagara->Activate(true);
			}
			ActualizarLongitudRayo();
		}
		else
		{
			EfectoRayoNiagara->SetVisibility(false);
			MallaRayo->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ARayoVigia::ActualizarLongitudRayo()
{
	if (!MallaRayo) return;
	MallaRayo->SetHiddenInGame(true);

	FVector OriginLocation = GetActorLocation();
	FVector DireccionDisparo = GetActorForwardVector();

	float OffsetInicio = 0.f;
	FVector StartLocation = OriginLocation + (DireccionDisparo * OffsetInicio);
	FVector EndLocation = StartLocation + (DireccionDisparo * LongitudMaxima);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.bTraceComplex = false;

	if (GetOwner())
	{
		CollisionParams.AddIgnoredActor(GetOwner());
		TArray<AActor*> Hijos;
		GetOwner()->GetAttachedActors(Hijos);
		CollisionParams.AddIgnoredActors(Hijos);
	}

	float LongitudActual = LongitudMaxima;
	FCollisionShape FormaRayo = FCollisionShape::MakeSphere(15.0f);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	TArray<FHitResult> HitResults;

	if (GetWorld()->SweepMultiByObjectType(HitResults, StartLocation, EndLocation, FQuat::Identity, ObjectParams, FormaRayo, CollisionParams))
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* ActorGolpeado = Hit.GetActor();

			if (ActorGolpeado && ActorGolpeado->IsA(ACentCosmosPawn::StaticClass()))
			{
				ACentCosmosPawn* NaveImpactada = Cast<ACentCosmosPawn>(ActorGolpeado);
				LongitudActual = Hit.Distance;

				if (bPuedeHacerDanio)
				{
					if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("¡Nave detectada por el Vigia! Aplicando Daño..."));

					NaveImpactada->RecibirDanioNave(15.0f);
					bPuedeHacerDanio = false;
					GetWorld()->GetTimerManager().SetTimer(TimerCooldownRayo, this, &ARayoVigia::ResetearDanioRayo, 2.0f, false);
				}
				break;
			}
		}
	}

	float FactorEscalaY = LongitudActual / 100.f;
	float DesplazamientoVisualY = (LongitudActual * 0.5f) + OffsetInicio;

	MallaRayo->SetRelativeScale3D(FVector(0.15f, FactorEscalaY, 0.15f));
	MallaRayo->SetRelativeLocation(FVector(DesplazamientoVisualY, 0.f, 0.f));

	EfectoRayoNiagara->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	EfectoRayoNiagara->SetRelativeLocation(FVector(OffsetInicio, 0.f, 0.f));
}

void ARayoVigia::ResetearDanioRayo()
{
	bPuedeHacerDanio = true;
}

