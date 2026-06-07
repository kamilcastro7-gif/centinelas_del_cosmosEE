#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Patterns/Builder/NivelDirector.h"       
#include "Patterns/Builder/Nivel1Builder.h"   
#include "Facade.h"                               
#include "CentCosmosGameMode.generated.h"

UCLASS()
class CENTCOSMOS_API ACentCosmosGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    ACentCosmosGameMode();
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void PostLogin(APlayerController* NewPlayer) override;

private:
    UPROPERTY() ANivelDirector* Director;
    UPROPERTY() ANivel1Builder* BuilderFacil;
    UPROPERTY() AFacade* ManejadorHorda;

    FTimerHandle TimerHandle_InputFix;
    void RestaurarInputJugador();
};