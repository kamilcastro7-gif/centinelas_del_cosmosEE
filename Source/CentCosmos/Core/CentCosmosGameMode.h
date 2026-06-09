#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CentCosmosPawn.h"
#include "NivelDirector.h"
#include "Nivel1Builder.h"
#include "Nivel2Builder.h"
#include "Nivel3Builder.h"
#include "Nivel4Builder.h"
#include "Nivel5Builder.h"
#include "Nivel6Builder.h"
#include "Facade.h"
#include "CentCosmosGameMode.generated.h"

UCLASS()
class CENTCOSMOS_API ACentCosmosGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    ACentCosmosGameMode();
    virtual void BeginPlay() override;
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY() ANivelDirector* Director;
    UPROPERTY() TScriptInterface<INivelBuilder> BuilderActivo;
    UPROPERTY() AFacade* ManejadorHorda;

    FTimerHandle TimerHandle_InputFix;
    void RestaurarInputJugador();
    TScriptInterface<INivelBuilder> CrearBuilderParaMapa(UWorld* Mundo, float& OutDificultad);
};