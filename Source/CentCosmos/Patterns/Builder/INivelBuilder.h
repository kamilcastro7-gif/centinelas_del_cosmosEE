#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FNivel.h"
#include "INivelBuilder.generated.h"

UINTERFACE(MinimalAPI)
class UNivelBuilder : public UInterface
{
    GENERATED_BODY()
};

class CENTCOSMOS_API INivelBuilder
{
    GENERATED_BODY()

public:
    virtual void Reset() = 0;
    virtual void SetMetadatos(const FString& Nombre, float TiempoLimite) = 0;
    virtual void SetDificultad(float Dificultad) = 0;
    virtual void AgregarEnemigos(UWorld* World, const TArray<AActor*>& Plantillas) = 0;
    virtual void AgregarBoss(UWorld* World, AActor* Boss) = 0;
    virtual FNivel ObtenerNivel() = 0;
};