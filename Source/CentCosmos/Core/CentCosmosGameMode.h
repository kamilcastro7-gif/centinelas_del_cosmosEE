#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CentCosmosGameMode.generated.h"

// Forward Declaration indispensable
class AFacade;

UCLASS()
class CENTCOSMOS_API ACentCosmosGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ACentCosmosGameMode();
    virtual void BeginPlay() override;
    virtual void PostLogin(APlayerController* NewPlayer) override;

    UFUNCTION(BlueprintCallable, Category = "Gestión de Niveles")
    void IniciarNivel(int32 NumeroNivel);

protected:
    UPROPERTY()
    AFacade* FacadeMaestro;

private:
    FTimerHandle TimerHandle_InputFix;
    void RestaurarInputJugador();
};



