#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IObserver.h"
#include "ISubject.h"
#include "Subject.generated.h"

UCLASS()
class CENTCOSMOS_API ASubject : public AActor, public ISubjectObservable
{
    GENERATED_BODY()
public:
    virtual void AddObserver(IObserver* Observer) override;
    virtual void RemoveObserver(IObserver* Observer) override;
    virtual void NotifyObservers(FName EventType, float Valor) override;

private:
    UPROPERTY()
    TArray<TScriptInterface<IObserver>> Observers;
};
