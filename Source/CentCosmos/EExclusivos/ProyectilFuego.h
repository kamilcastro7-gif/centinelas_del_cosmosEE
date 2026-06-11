// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilFuego.generated.h"

class UNiagaraComponent; // NUEVO: Para el sistema de partículas Niagara

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
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	// --- HITBOX INVISIBLE ---
	UPROPERTY(VisibleAnywhere, Category = "Componentes")
	class UStaticMeshComponent* ProyectilMesh;

	// --- EFECTO VISUAL NIAGARA ---
	UPROPERTY(VisibleAnywhere, Category = "Componentes")
	UNiagaraComponent* EfectoFuego;

	float Velocidad;
	FTimerHandle TimerRastrosHandle;

	void SoltarRastro();
};




