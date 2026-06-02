#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INivelBuilder.h"
#include "NivelMedioBuilder.generated.h"

UCLASS()
class CENTCOSMOS_API ANivelMedioBuilder : public AActor, public INivelBuilder
{
    GENERATED_BODY()

public:
    ANivelMedioBuilder();

    virtual void Reset() override;
    virtual void SetMetadatos(const FString& Nombre, float TiempoLimite) override;
    virtual void SetDificultad(float Dificultad) override;
    virtual void AgregarEnemigos(UWorld* World) override;
    virtual void AgregarAmbientacion(UWorld* World) override;
    virtual FNivel ObtenerNivel() override;
    void AgregarBoss(UWorld* World);
private:
    FNivel NivelActual;
    TArray<TWeakObjectPtr<AActor>> EnemigosGenerados;
};