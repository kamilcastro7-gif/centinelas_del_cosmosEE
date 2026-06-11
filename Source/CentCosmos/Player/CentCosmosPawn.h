// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Patterns/Decorator/EnemDecorador.h"
#include "Observer/VidaObserver.h"
#include "Observer/Subject.h"
#include "CentCosmosPawn.generated.h"

UENUM()
enum class ETipoArma : uint8
{
	Normal,
	Boomerang,
	Carga
};

UCLASS()
class CENTCOSMOS_API ACentCosmosPawn : public APawn
{
	GENERATED_BODY()

	// Mantenemos los macros de Blueprint por defecto del engine para la cámara y la malla
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	ACentCosmosPawn();

	virtual void BeginPlay() override;

	UPROPERTY(Category = Gameplay, EditAnywhere)
	FVector GunOffset;

	UPROPERTY(Category = Gameplay, EditAnywhere)
	float FireRate;

	UPROPERTY(Category = Gameplay, EditAnywhere)
	float MoveSpeed;

	UPROPERTY(Category = "Audio", EditAnywhere)
	class USoundBase* FireSound;

	UPROPERTY()
	UEnemDecorador* Decorador;

	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void FireShot(FVector FireDirection);
	void ShotTimerExpired();
	void DesactivarDisparoTriple();
	void DesactivarSobreCargaApex();

	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;

	UPROPERTY(VisibleAnywhere, Category = "Armas")
	ETipoArma ArmaActual;

	UPROPERTY(VisibleAnywhere, Category = "Armas")
	bool bBoomerangEnVuelo;

	// SISTEMA DE SALUD
	UPROPERTY(EditAnywhere, Category = "Salud")
	float VidaMax;

	UPROPERTY(VisibleAnywhere, Category = "Salud")
	float VidaActual;

	void RecibirDanioNave(float Cantidad);
	void RestaurarVida(float Cantidad);

private:
	uint32 bCanFire : 1;

	FTimerHandle TimerHandle_ShotTimerExpired;
	FTimerHandle TimerHandle_DisparoTriple;
	FTimerHandle TimerHandle_SobreCargaApex;

	// Observer
	UPROPERTY()
	class ASubject* SubjectVida;

	UPROPERTY()
	class UVidaObserver* ObservadorVida;

	float TiempoCargaAcumulado;
	bool  bEstaCargando;

	UPROPERTY()
	class AProyectilCarga* ProyectilCargaActual;

public:
	UPROPERTY(EditAnywhere, Category = "PowerUps")
	bool bTieneDisparoTriple;

	UPROPERTY(EditAnywhere, Category = "PowerUps")
	bool bTieneSobreCargaApex;

	UPROPERTY(EditAnywhere, Category = "Armas")
	TSubclassOf<class ACentCosmosProjectile> ClaseNormal;

	UPROPERTY(EditAnywhere, Category = "Armas")
	TSubclassOf<class AProyectilCarga> ClaseCarga;

	float MoveSpeedBase;
	float FireRateBase;

	bool bRalentizadoPorChispa = false;
	bool bPuedeDisparar = true;

	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
	FORCEINLINE class UCameraComponent* GetCameraComponent()   const { return CameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom()        const { return CameraBoom; }
};