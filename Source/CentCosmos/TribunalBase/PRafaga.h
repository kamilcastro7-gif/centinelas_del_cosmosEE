// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PRafaga.generated.h"

class UNiagaraComponent; // NUEVO: Declaración para el sistema de partículas Niagara

UCLASS()
class CENTCOSMOS_API APRafaga : public AActor
{
	GENERATED_BODY()

public:
	APRafaga();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// --- HITBOX INVISIBLE ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class UStaticMeshComponent* MallaProyectil;

	// --- EFECTO VISUAL NIAGARA ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	UNiagaraComponent* EfectoNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
	float Velocidad;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
