#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CentCosmosGameMode.generated.h"

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

    // true = jugador ganó (jefe muerto), false = jugador perdió (nave destruida)
    UFUNCTION(BlueprintCallable, Category = "Gestión de Niveles")
    void OnJefeDerrotado(bool bJugadorVivo = true);

    UPROPERTY(EditDefaultsOnly, Category = "Winner_L")
    FName NombreNivelGanador = FName("Winner");

    UPROPERTY(EditDefaultsOnly, Category = "GameOver_L")
    FName NombreNivelGameOver = FName("GameOver");

protected:
    UPROPERTY()
    AFacade* FacadeMaestro;

private:
    FTimerHandle TimerHandle_InputFix;
    void RestaurarInputJugador();
};