// Fill out your copyright notice in the Description page of Project Settings.


#include "ProyectilBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AProyectilBase::AProyectilBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Crear Malla
	MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaProyectil"));
	RootComponent = MallaProyectil;

	// Cargar una esfera básica del motor por defecto
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (MeshAsset.Succeeded()) {
		MallaProyectil->SetStaticMesh(MeshAsset.Object);
	}

	// Configurar Movimiento
	MovimientoProyectil = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovimientoProyectil"));
	MovimientoProyectil->bRotationFollowsVelocity = true;
	MovimientoProyectil->ProjectileGravityScale = 0.0f; // Sin gravedad para el espacio

	// Valores por defecto
	VidaProyectil = 1;
	bEsSeguidor = false;

	// Por defecto, desaparece en 5 segundos si no choca con nada
	InitialLifeSpan = 5.0f;
}

void AProyectilBase::BeginPlay() {
	Super::BeginPlay();
}

void AProyectilBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// Movimiento constante para TODOS los proyectiles
	FVector NuevaUbicacion = GetActorLocation() + (GetActorForwardVector() * MovimientoProyectil->InitialSpeed * DeltaTime);
	SetActorLocation(NuevaUbicacion);

	// LÓGICA DE PERSECUCIÓN (Solo para el misil del Heraldo)
	if (bEsSeguidor) {
		AActor* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (Jugador) {
			// Girar suavemente hacia el jugador
			FRotator RotacionObjetivo = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Jugador->GetActorLocation());
			FRotator NuevaRotacion = FMath::RInterpTo(GetActorRotation(), RotacionObjetivo, DeltaTime, 2.0f); // Velocidad de giro: 2.0
			SetActorRotation(NuevaRotacion);
		}
	}
}

void AProyectilBase::RecibirImpacto() {
	VidaProyectil--;

	// OPCIONAL: Esto te avisará en la consola de Unreal cuánta vida le queda
	// UE_LOG(LogTemp, Warning, TEXT("Impacto al proyectil! Vida restante: %d"), VidaProyectil);

	if (VidaProyectil <= 0) {
		Destroy();
	}
}

