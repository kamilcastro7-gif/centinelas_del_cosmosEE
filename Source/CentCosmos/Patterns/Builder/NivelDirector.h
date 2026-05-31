#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INivelBuilder.h"
#include "NivelDirector.generated.h"

UCLASS()
class CENTCOSMOS_API ANivelDirector : public AActor
{
    GENERATED_BODY()

public:
    ANivelDirector();

    void SetBuilder(TScriptInterface<INivelBuilder> NuevoBuilder);

    FNivel ConstruirNivelCompleto(UWorld* World, const FString& Nombre, float TiempoLimite, float Dificultad, const TArray<AActor*>& Plantillas, AActor* Boss);

    FNivel ConstruirTutorial(UWorld* World, const TArray<AActor*>& Plantillas);

    FNivel ConstruirOleada(UWorld* World, const TArray<AActor*>& Plantillas);

private:
    TScriptInterface<INivelBuilder> Builder;
};