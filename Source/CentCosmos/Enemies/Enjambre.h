#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Patterns/Decorator/EnemDecorador.h" // <-- 1. Añadir el include
#include "Enjambre.generated.h"

UCLASS()
class CENTCOSMOS_API AEnjambre : public AActor
{
	GENERATED_BODY()

public:
	AEnjambre();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MallaEnemigo;

	FVector DireccionMovimiento;
	float ProbabilidadDrop;

	void DropPowerUp();

public:
	virtual void Tick(float DeltaTime) override;

	// Estadísticas base
	float VidaActual;
	float DanioDeChoque;

	// <-- 2. Declarar el componente Decorador
	UPROPERTY()
	UEnemDecorador* DecoradorVida;

	virtual void RecibirDanioEnemigo(float Cantidad);
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};