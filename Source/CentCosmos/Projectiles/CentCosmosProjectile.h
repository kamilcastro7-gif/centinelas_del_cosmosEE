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

	UPROPERTY()
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;

public:
	ACentCosmosProjectile();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ForzarDireccion(FVector Direccion, float Velocidad);

	// ATRIBUTO DE DA�O
	UPROPERTY()
	float Danio;

	FORCEINLINE UStaticMeshComponent* GetProjectileMesh()      const { return ProjectileMesh; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement()  const { return ProjectileMovement; }
};
