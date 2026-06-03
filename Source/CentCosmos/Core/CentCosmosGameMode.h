#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Patterns/Builder/NivelDirector.h"       
#include "Patterns/Builder/NivelFacilBuilder.h"   
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

private:
    // Builder
    UPROPERTY() ANivelDirector* Director;
    UPROPERTY() ANivelFacilBuilder* BuilderFacil;
    UPROPERTY() AFacade* ManejadorHorda;


};