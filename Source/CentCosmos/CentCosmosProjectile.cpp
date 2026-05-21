// Copyright Epic Games, Inc. All Rights Reserve

#include "CentCosmosProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

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

// Fuerza la dirección y velocidad del proyectil desde afuera
void ACentCosmosProjectile::ForzarDireccion(FVector Direccion, float Velocidad)
{
	if (ProjectileMovement)
	{
		FVector Dir = Direccion.GetSafeNormal();
		ProjectileMovement->Velocity = Dir * Velocidad;
		ProjectileMovement->MaxSpeed = Velocidad;
		ProjectileMovement->UpdateComponentVelocity();
		SetActorRotation(Dir.Rotation());
	}
}

void ACentCosmosProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)
		&& OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 20.f, GetActorLocation());
	}
	Destroy();
}