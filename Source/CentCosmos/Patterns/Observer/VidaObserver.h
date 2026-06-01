#pragma once

#include "CoreMinimal.h"
#include "IObserver.h"
#include "VidaObserver.generated.h"

UCLASS(Blueprintable, BlueprintType)
class CENTCOSMOS_API UVidaObserver : public UObject, public IObserver
{
    GENERATED_BODY()

public:
    UVidaObserver();

    virtual void OnNotify(FName EventType) override;

    UPROPERTY(EditAnywhere, Category = "Vida")
    float UmbralVidaCritica;
};