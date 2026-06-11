#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INivelBuilder.h"
#include "FNivel.h"
#include "NivelDirector.generated.h"

UCLASS()
class CENTCOSMOS_API ANivelDirector : public AActor
{
    GENERATED_BODY()
public:
    ANivelDirector();

    void SetBuilder(TScriptInterface<INivelBuilder> NuevoBuilder);

    FNivel ConstruirNivel(UWorld* World, const FString& Nombre, float TiempoLimite, float Dificultad);
    FNivel ConstruirTutorial(UWorld* World);
    FNivel ConstruirOleada(UWorld* World, float Dificultad);

private:
    UPROPERTY() TScriptInterface<INivelBuilder> Builder;
};