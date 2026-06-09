#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NivelDirector.h"
#include "Nivel1Builder.h"
#include "LV01_GameMode.generated.h"

UCLASS()
class CENTCOSMOS_API ALV01_GameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    ALV01_GameMode();
    virtual void BeginPlay() override;
private:
    UPROPERTY()
    ANivelDirector* Director;
    UPROPERTY()
    ANivel1Builder* Builder;
};