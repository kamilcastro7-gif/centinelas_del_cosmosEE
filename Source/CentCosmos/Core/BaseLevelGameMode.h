#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CentCosmosPawn.h"
#include "BaseLevelGameMode.generated.h"

UCLASS()
class CENTCOSMOS_API ABaseLevelGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    ABaseLevelGameMode();
    virtual void BeginPlay() override;
    virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
    FTimerHandle TimerHandle_InputFix;

private:
    void RestaurarInputJugador();
};