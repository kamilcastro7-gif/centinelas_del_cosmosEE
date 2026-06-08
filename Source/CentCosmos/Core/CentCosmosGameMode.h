#pragma once
#include "CoreMinimal.h"
#include "BaseLevelGameMode.h"
#include "CentCosmosPawn.h"
#include "NivelDirector.h"
#include "Nivel1Builder.h"
#include "Facade.h"
#include "CentCosmosGameMode.generated.h"

UCLASS()
class CENTCOSMOS_API ACentCosmosGameMode : public ABaseLevelGameMode
{
    GENERATED_BODY()
public:
    ACentCosmosGameMode();
    virtual void BeginPlay() override;
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY()
    ANivelDirector* Director;
    UPROPERTY()
    ANivel1Builder* BuilderFacil;
    UPROPERTY()
    AFacade* ManejadorHorda;

    FTimerHandle TimerHandle_InputFix;
    void RestaurarInputJugador();
};