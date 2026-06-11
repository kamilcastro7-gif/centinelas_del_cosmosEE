// Fill out your copyright notice in the Description page of Project Settings.


#include "ProyectilBoomerang.h"
#include "Components/StaticMeshComponent.h"
#include "Centinela_Del_CosmosPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "ProyectilBase.h" // Incluido para poder detectar y dañar el proyectil del Heraldo al pasar

AProyectilBoomerang::AProyectilBoomerang()
{
	PrimaryActorTick.bCanEverTick = true;

	// Asignación de la malla de la dona (Shape_Torus)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TorusMesh(TEXT("/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus"));
	ProyectilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoomerangMesh"));
	RootComponent = ProyectilMesh;

	if (TorusMesh.Succeeded())
	{
		ProyectilMesh->SetStaticMesh(TorusMesh.Object);
	}

	// Configuración de solapamiento (Overlap) para atravesar enemigos y objetos sin frenar
	ProyectilMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ProyectilMesh->OnComponentBeginOverlap.AddDynamic(this, &AProyectilBoomerang::OnOverlap);

	Velocidad = 1500.f;
	TiempoDeGiro = 1.2f;
	TiempoEnVuelo = 0.f;
	bRegresando = false;
}

void AProyectilBoomerang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TiempoEnVuelo += DeltaTime;

	// Si cumple su tiempo de ida, cambia el estado para activar el retorno magnético
	if (!bRegresando && TiempoEnVuelo >= TiempoDeGiro)
	{
		bRegresando = true;
	}

	if (!bRegresando)
	{
		// Trayectoria recta hacia adelante
		FVector NuevaPosicion = GetActorLocation() + (GetActorForwardVector() * Velocidad * DeltaTime);
		SetActorLocation(NuevaPosicion);
	}
	else if (NaveDueno)
	{
		// Retorno magnético persiguiendo la posición exacta de la nave en movimiento
		FVector DireccionALaNave = (NaveDueno->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FVector NuevaPosicion = GetActorLocation() + (DireccionALaNave * Velocidad * DeltaTime);

		AddActorLocalRotation(FRotator(0.f, 5.f, 0.f)); // Giro visual continuo de la dona estilo bumerán
		SetActorLocation(NuevaPosicion);
	}
}

void AProyectilBoomerang::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		// INTERACCIÓN DE DAÑO: Si cruza por encima de un proyectil del Heraldo (ProyectilBase)
		AProyectilBase* ProyectilEnemigo = Cast<AProyectilBase>(OtherActor);
		if (ProyectilEnemigo)
		{
			// Destruye o daña la bala enemiga pero el bumerán NO se destruye (sigue de largo reventando todo)
			ProyectilEnemigo->RecibirImpacto();
		}

		// Lógica de retorno e interacción final con el jugador
		if (bRegresando && OtherActor == NaveDueno)
		{
			if (NaveDueno)
			{
				NaveDueno->bBoomerangEnVuelo = false; // Desbloquea el disparo de bumerán en la nave
			}
			Destroy();
		}
	}
}

