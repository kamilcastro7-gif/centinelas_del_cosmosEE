#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Facade.h"
#include "CentCosmosGameMode.generated.h"

UCLASS()
class CENTCOSMOS_API ACentCosmosGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ACentCosmosGameMode();

    void IniciarNivel(int32 NumeroNivel);

protected:
    virtual void BeginPlay() override;
    virtual void PostLogin(APlayerController* NewPlayer) override;

private:
    UPROPERTY()
    AFacade* FacadeMaestro;
};