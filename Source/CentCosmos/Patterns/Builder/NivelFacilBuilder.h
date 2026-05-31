#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INivelBuilder.h"
#include "Patterns/Decorator/IEnemigo.h"
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
    virtual void AgregarEnemigos(UWorld* World, const TArray<AActor*>& Plantillas) override;
    virtual void AgregarBoss(UWorld* World, AActor* Boss) override;
    virtual FNivel ObtenerNivel() override;

private:
    FNivel NivelActual;
    TArray<TWeakObjectPtr<AActor>> EnemigosGenerados;
};