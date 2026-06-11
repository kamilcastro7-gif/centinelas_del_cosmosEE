// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CentCosmosProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class UNiagaraComponent; // NUEVO: Para el sistema de partículas Niagara

UCLASS(config = Game)
class ACentCosmosProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = "Componentes")
	UNiagaraComponent* EfectoNiagara;

	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;

public:
	ACentCosmosProjectile();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ForzarDireccion(FVector Direccion, float Velocidad);

	float Danio;

	FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
