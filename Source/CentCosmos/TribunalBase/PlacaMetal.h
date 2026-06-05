// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "PlacaMetal.generated.h"

UCLASS()
class CENTCOSMOS_API APlacaMetal : public AActor
{
	GENERATED_BODY()

public:
	APlacaMetal();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	UStaticMeshComponent* MallaPlaca;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atributos")
	float VidaMax;

	UPROPERTY(BlueprintReadOnly, Category = "Atributos")
	float VidaActual;

	UFUNCTION(BlueprintCallable, Category = "Combate")
	void RecibirDanioPlaca(float CantidadDanio);

	// === BLINDAJE: Para avisar al jefe sin crashear ===
	UPROPERTY()
	AActor* OwnerJefe;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
