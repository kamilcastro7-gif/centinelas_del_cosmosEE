// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INivelBuilder.h"
#include "NivelFacilBuilder.generated.h"

UCLASS()
class CENTCOSMOS_API ANivelFacilBuilder : public AActor, public INivelBuilder
{
	GENERATED_BODY()

public:
	ANivelFacilBuilder();

	virtual void Reset() override;
	virtual void SetMetadatos(const FString& Nombre, float TiempoLimite) override;
	virtual void SetDificultad(float Dificultad) override;
	virtual void AgregarEnemigos(UWorld* World) override;
	virtual void AgregarAmbientacion(UWorld* World) override;
	virtual FNivel ObtenerNivel() override;

	// Tick para monitorear si todos los enemigos de ola 1 murieron
	virtual void Tick(float DeltaTime) override;

private:
	FNivel NivelActual;
	TArray<TWeakObjectPtr<AActor>> EnemigosGenerados;

	// Enemigos de la primera ola — monitoreamos sus muertes
	TArray<TWeakObjectPtr<AActor>> EnemigosOla1;

	// Flag para no spawnear el boss mas de una vez
	bool bBossSpawneado;

	// Verifica si todos los enemigos de ola1 estan muertos y spawna el boss
	void VerificarYSpawnearBoss(UWorld* World);
};