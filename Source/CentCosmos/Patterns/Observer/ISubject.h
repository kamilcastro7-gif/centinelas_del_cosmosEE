#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IObserver.h"
#include "ISubject.generated.h"

UINTERFACE(MinimalAPI)
class USubjectObservable : public UInterface { GENERATED_BODY() };

class CENTCOSMOS_API ISubjectObservable
{
    GENERATED_BODY()
public:
    virtual void AddObserver(IObserver* Observer) = 0;
    virtual void RemoveObserver(IObserver* Observer) = 0;
    virtual void NotifyObservers(FName EventType, float Valor) = 0;
};
