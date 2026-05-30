// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CentCosmosPawn.generated.h"

UENUM(BlueprintType)
enum class ETipoArma : uint8
{
	Normal,
	Boomerang,
	Carga
};

UCLASS(Blueprintable)
class CENTCOSMOS_API ACentCosmosPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	ACentCosmosPawn();

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	FVector GunOffset;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
	class USoundBase* FireSound;

	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void FireShot(FVector FireDirection);
	void ShotTimerExpired();

	class ACentCosmosProjectile* SpawnProyectilNormal(
		UWorld* World, FVector Location, FRotator Rotacion, float Velocidad);

	void DesactivarDisparoTriple();
	void DesactivarSobreCargaApex();

	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;

	UPROPERTY(BlueprintReadOnly, Category = "Armas")
	ETipoArma ArmaActual;

	UPROPERTY(BlueprintReadWrite, Category = "Armas")
	bool bBoomerangEnVuelo;

	// =========================================================================
	// SISTEMA DE SALUD
	// =========================================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Salud")
	float VidaMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Salud")
	float VidaActual;

	UFUNCTION(BlueprintCallable, Category = "Salud")
	void RecibirDanioNave(float Cantidad);

private:
	uint32 bCanFire : 1;

	FTimerHandle TimerHandle_ShotTimerExpired;
	FTimerHandle TimerHandle_DisparoTriple;
	FTimerHandle TimerHandle_SobreCargaApex;

	float TiempoCargaAcumulado;
	bool  bEstaCargando;

	UPROPERTY()
	class AProyectilCarga* ProyectilCargaActual;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUps")
	bool bTieneDisparoTriple;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUps")
	bool bTieneSobreCargaApex;

	UPROPERTY(EditAnywhere, Category = "Configuracion Disparo")
	TSubclassOf<class ACentCosmosProjectile> ClaseBalaBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armas")
	TSubclassOf<class ACentCosmosProjectile> MiProyectilAmarilloBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armas")
	TSubclassOf<class ACentCosmosProjectile> ClaseNormalBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armas")
	TSubclassOf<class AProyectilCarga> ClaseCargaBP;

	float MoveSpeedBase;
	float FireRateBase;

public:
	bool bRalentizadoPorChispa = false;
	bool bPuedeDisparar = true;

public:
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
	FORCEINLINE class UCameraComponent* GetCameraComponent()   const { return CameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom()        const { return CameraBoom; }
};

