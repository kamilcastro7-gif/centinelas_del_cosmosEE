// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CentCosmosProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS(config = Game)
class ACentCosmosProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:
	ACentCosmosProjectile();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ForzarDireccion(FVector Direccion, float Velocidad);

	// ATRIBUTO DE DAÑO
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate")
	float Danio;

	FORCEINLINE UStaticMeshComponent* GetProjectileMesh()      const { return ProjectileMesh; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement()  const { return ProjectileMovement; }
};
