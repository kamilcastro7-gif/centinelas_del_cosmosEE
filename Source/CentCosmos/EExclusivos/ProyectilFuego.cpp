// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilFuego.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "RastrosFuego.h"
#include "Engine/World.h"
#include "../Player/CentCosmosPawn.h" // NECESARIO PARA EL DAÑO

AProyectilFuego::AProyectilFuego()
{
	PrimaryActorTick.bCanEverTick = true;
	ProyectilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProyectilMesh"));
	RootComponent = ProyectilMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (CubeMesh.Succeeded()) ProyectilMesh->SetStaticMesh(CubeMesh.Object);

	SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
	Velocidad = 1100.0f;

	ProyectilMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProyectilMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AProyectilFuego::BeginPlay()
{
	AActor::BeginPlay();
	SetLifeSpan(3.0f);
	AActor* Dueno = GetOwner();
	if (Dueno && ProyectilMesh) ProyectilMesh->IgnoreActorWhenMoving(Dueno, true);
	GetWorld()->GetTimerManager().SetTimer(TimerRastrosHandle, this, &AProyectilFuego::SoltarRastro, 0.12f, true);
}

void AProyectilFuego::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
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

// --- NUEVO: APLICAR DAÑO AL IMPACTO DIRECTO ---
void AProyectilFuego::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
	{
		ACentCosmosPawn* Nave = Cast<ACentCosmosPawn>(OtherActor);
		if (Nave)
		{
			Nave->RecibirDanioNave(8.0f); // DAÑO SEGÚN LISTA
			Destroy();
		}
	}
}

