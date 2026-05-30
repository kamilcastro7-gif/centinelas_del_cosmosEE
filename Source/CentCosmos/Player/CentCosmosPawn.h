// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Patterns/Facade/FachadaEstadosJugador.h"
#include "Patterns/Observer/IObserver.h"
#include "CentCosmosPawn.generated.h"

UENUM(BlueprintType)
enum class ETipoArma : uint8
{
	Normal,
	Boomerang,
	Carga
};

UCLASS(Blueprintable)
class CENTCOSMOS_API ACentCosmosPawn : public APawn, public IObserver
{
	GENERATED_BODY()

	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	/* The camera */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/* The camera boom positioning the camera above the character */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	ACentCosmosPawn();

	/** Offset from the ships location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	FVector GunOffset;

	/** How fast the weapon fires */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float FireRate;

	/** The speed of the ship */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
	class USoundBase* FireSound;

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void OnNotify(FName EventType) override;
	// End Actor Interface

	/* Handler for the fire timer expiring */
	void FireShot(FVector FireDirection);
	void ShotTimerExpired();

	// Static names for the input bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;

	UPROPERTY(BlueprintReadOnly, Category = "Armas")
	ETipoArma ArmaActual;

	UPROPERTY(BlueprintReadWrite, Category = "Armas")
	bool bBoomerangEnVuelo;

private:
	/* Flag to control firing  */
	uint32 bCanFire : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

	float TiempoCargaAcumulado;
	bool bEstaCargando;

	UPROPERTY()
	UFachadaEstadosJugador* Fachada;

	UPROPERTY()
	class AProyectilCarga* ProyectilCargaActual;

public:
	/** Returns ShipMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
	/** Returns CameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};

