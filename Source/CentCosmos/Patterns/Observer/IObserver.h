// IObserver.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IObserver.generated.h"

UINTERFACE(MinimalAPI)
class UObserver : public UInterface
{
    GENERATED_BODY()
};

class CENTCOSMOS_API IObserver
{
    GENERATED_BODY()
public:
    virtual void OnNotify(FName EventType) = 0;
};
