// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilCarga.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "ProyectilBase.h"

AProyectilCarga::AProyectilCarga()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	ProyectilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereCargaMesh"));
	RootComponent = ProyectilMesh;

	if (SphereMesh.Succeeded())
	{
		ProyectilMesh->SetStaticMesh(SphereMesh.Object);
	}

	// Configuramos colisiones e ignoramos el canal de la nave (Pawn) para evitar frenados
	ProyectilMesh->SetCollisionProfileName(TEXT("Projectile"));
	ProyectilMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = ProyectilMesh;

	// SOLUCIÓN AL PROYECTIL ESTÁTICO: Iniciamos la velocidad en 0 y pausamos la simulación
	// Esto evita que el componente de movimiento se rompa o se rinda mientras la bola está soldada a la nave
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 4000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bSimulationEnabled = false;

	DanoBase = 10.f;
	InitialLifeSpan = 4.0f;
}

void AProyectilCarga::InicializarCarga(float TiempoCarga)
{
	// Ajustamos el tamaño visual de la esfera según el tiempo acumulado
	if (TiempoCarga >= 6.0f)
	{
		SetActorScale3D(FVector(1.8f));
	}
	else if (TiempoCarga >= 3.0f)
	{
		SetActorScale3D(FVector(1.2f));
	}
	else
	{
		float ProgresoEscala = FMath::Lerp(0.4f, 1.0f, TiempoCarga / 3.0f);
		SetActorScale3D(FVector(ProgresoEscala));
	}
}

void AProyectilCarga::LiberarProyectil(float TiempoCargaFinal, FVector DireccionLanzamiento)
{
	float VelocidadLanzamiento = 2500.f;

	// Definimos el daño y velocidad final dependiendo del nivel de carga alcanzado
	if (TiempoCargaFinal >= 6.0f)
	{
		DanoBase = 100.f;
		VelocidadLanzamiento = 2000.f; // Proyectil masivo y devastador, un poco más lento
	}
	else if (TiempoCargaFinal >= 3.0f)
	{
		DanoBase = 45.f;
		VelocidadLanzamiento = 2700.f;
	}
	else
	{
		DanoBase = 10.f;
		VelocidadLanzamiento = 3400.f; // Disparo rápido de emergencia
	}

	// FORZADO DE FÍSICAS: Despertamos el componente de movimiento y aplicamos el vector de empuje directo
	if (ProjectileMovement)
	{
		ProjectileMovement->bSimulationEnabled = true; // Activamos la simulación física
		ProjectileMovement->InitialSpeed = VelocidadLanzamiento;
		ProjectileMovement->Velocity = DireccionLanzamiento * VelocidadLanzamiento;

		// Obligamos a Unreal a actualizar y refrescar los cambios de velocidad en este frame
		ProjectileMovement->SetVelocityInLocalSpace(FVector(VelocidadLanzamiento, 0.f, 0.f));
		ProjectileMovement->UpdateComponentVelocity();
	}

	// Enlazamos dinámicamente el evento de impacto justo ahora que el proyectil se mueve de verdad
	ProyectilMesh->OnComponentHit.AddDynamic(this, &AProyectilCarga::AlChocar);
}

void AProyectilCarga::AlChocar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		AProyectilBase* ProyectilEnemigo = Cast<AProyectilBase>(OtherActor);
		if (ProyectilEnemigo)
		{
			ProyectilEnemigo->RecibirImpacto();
			Destroy();
			return;
		}
	}
	Destroy();
}

