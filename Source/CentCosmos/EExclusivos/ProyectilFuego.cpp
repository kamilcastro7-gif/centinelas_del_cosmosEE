// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilFuego.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "RastrosFuego.h"
#include "Engine/World.h"
#include "../Player/CentCosmosPawn.h" 
// --- NUEVOS INCLUDES PARA NIAGARA ---
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AProyectilFuego::AProyectilFuego()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. EL TRUCO DE LA HITBOX: Creamos el cubo base
	ProyectilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProyectilMesh"));
	RootComponent = ProyectilMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (CubeMesh.Succeeded()) ProyectilMesh->SetStaticMesh(CubeMesh.Object);

	// Lo volvemos invisible en el juego, pero mantiene la colisión
	ProyectilMesh->SetHiddenInGame(true);
	ProyectilMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProyectilMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
	Velocidad = 1100.0f;

	// 2. EFECTO VISUAL: Añadimos el componente Niagara
	EfectoFuego = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoFuego"));
	EfectoFuego->SetupAttachment(RootComponent);

	// Cargamos el Asset de Niagara
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Hit_5.NS_Hit_5'"));
	if (NiagaraAsset.Succeeded())
	{
		EfectoFuego->SetAsset(NiagaraAsset.Object);
	}

	// Opcional: Ajustar escala si el fuego es muy grande o muy pequeño
	EfectoFuego->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void AProyectilFuego::BeginPlay()
{
	Super::BeginPlay(); // Usamos Super:: en lugar de AActor:: por buenas prácticas
	SetLifeSpan(3.0f);
	AActor* Dueno = GetOwner();
	if (Dueno && ProyectilMesh) ProyectilMesh->IgnoreActorWhenMoving(Dueno, true);
	GetWorld()->GetTimerManager().SetTimer(TimerRastrosHandle, this, &AProyectilFuego::SoltarRastro, 0.12f, true);
}

void AProyectilFuego::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector NuevaPosicion = GetActorLocation() + (GetActorForwardVector() * Velocidad * DeltaTime);
	SetActorLocation(NuevaPosicion);
}

void AProyectilFuego::SoltarRastro()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		World->SpawnActor<ARastrosFuego>(ARastrosFuego::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
	}
}

void AProyectilFuego::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
	{
		ACentCosmosPawn* Nave = Cast<ACentCosmosPawn>(OtherActor);
		if (Nave)
		{
			Nave->RecibirDanioNave(8.0f);
			Destroy();
		}
	}
}

