// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Escudo.generated.h"

UCLASS()
class CENTCOSMOS_API AEscudo : public AActor
{
	GENERATED_BODY()

public:
	AEscudo();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Escudo")
	UStaticMeshComponent* EscudoMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Escudo")
	float VidaEscudo;

	void RecibirDanoEscudo(float CantidadDano);
};
