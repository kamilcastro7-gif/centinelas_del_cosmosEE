// Copyright Epic Games, Inc. All Rights Reserve

#include "CentCosmosProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Escudo.h"
#include "GrietaAntimateria.h"

ACentCosmosProjectile::ACentCosmosProjectile()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(
		TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
	ProjectileMesh->OnComponentHit.AddDynamic(this, &ACentCosmosProjectile::OnHit);
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 6000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 3.0f;
}

void ACentCosmosProjectile::ForzarDireccion(FVector Direccion, float Velocidad)
{
	if (!ProjectileMovement) return;

	FVector Dir = Direccion.GetSafeNormal();

	// Paramos cualquier movimiento previo y reseteamos el componente
	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->SetActive(false);

	// Asignamos la nueva velocidad y límites
	ProjectileMovement->InitialSpeed = Velocidad;
	ProjectileMovement->MaxSpeed = Velocidad;
	ProjectileMovement->Velocity = Dir * Velocidad;

	// Reactivamos el componente para que procese la nueva velocidad
	ProjectileMovement->SetActive(true);
	ProjectileMovement->UpdateComponentVelocity();

	// Rotamos el actor para que apunte en la dirección correcta
	SetActorRotation(Dir.Rotation());
}

void ACentCosmosProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		// 1. Lógica del Escudo (la que ya tienes)
		AEscudo* EscudoImpactado = Cast<AEscudo>(OtherActor);
		if (EscudoImpactado != nullptr || OtherActor->GetName().Contains(TEXT("Escudo")))
		{
			if (EscudoImpactado == nullptr) EscudoImpactado = Cast<AEscudo>(OtherActor);
			if (EscudoImpactado != nullptr) EscudoImpactado->RecibirDanoEscudo(1.0f);
			Destroy();
			return;
		}

		// 2. NUEVA LÓGICA DE LA GRIETA
		AGrietaAntimateria* GrietaImpactada = Cast<AGrietaAntimateria>(OtherActor);
		if (GrietaImpactada != nullptr)
		{
			// Avisamos a la grieta antes de destruir la bala
			GrietaImpactada->ProcesarImpacto();
			Destroy();
			return;
		}
	}

	// Lógica de física para objetos movibles
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 20.f, GetActorLocation());
	}

	Destroy();
}