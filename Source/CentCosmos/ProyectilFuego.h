// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilFuego.generated.h"

UCLASS()
class CENTCOSMOS_API AProyectilFuego : public AActor
{
	GENERATED_BODY()

public:
	AProyectilFuego();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ProyectilMesh;

	float Velocidad;
	FTimerHandle TimerRastrosHandle;

	void SoltarRastro();
};
