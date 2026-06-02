#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IObserver.h"
#include "VidaObserver.generated.h"

UCLASS()
class CENTCOSMOS_API UVidaObserver : public UObject, public IObserver
{
    GENERATED_BODY()
public:
    virtual void OnNotify(FName EventType, float Valor) override;
};