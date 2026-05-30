// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombarderoFuego.generated.h"

UCLASS()
class CENTCOSMOS_API ABombarderoFuego : public AActor
{
	GENERATED_BODY()

public:
	ABombarderoFuego();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BombarderoMesh;

	float VelocidadMovimiento;
	float RangoBarrido;
	FVector PosicionCentro;
	bool bMoviendoDerecha;

	FTimerHandle TimerAtaqueHandle;
	void EjecutarDisparoFuego();
};
