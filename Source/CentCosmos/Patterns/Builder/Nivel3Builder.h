#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INivelBuilder.h"
#include "Nivel3Builder.generated.h"

UCLASS()
class CENTCOSMOS_API ANivel3Builder : public AActor, public INivelBuilder
{
    GENERATED_BODY()

public:
    ANivel3Builder();

	virtual void Reset() override;
	virtual void SetMetadatos(const FString& Nombre, float TiempoLimite) override;
	virtual void SetDificultad(float Dificultad) override;

	// A estas dos funciones QUÍTALES cualquier parámetro extra. Solo deben recibir UWorld*
	virtual void AgregarEnemigos(UWorld* World) override;
	virtual void AgregarAmbientacion(UWorld* World) override;

	virtual FNivel ObtenerNivel() override;

	// Si tienes AgregarBoss, QUÍTALE la palabra 'override' al final, 
	// ya que es una función exclusiva de esta clase, no de la interfaz.
	void AgregarBoss(UWorld* World);

private:
    FNivel NivelActual;
    TArray<TWeakObjectPtr<AActor>> EnemigosGenerados;
};
