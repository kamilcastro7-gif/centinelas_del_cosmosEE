#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IObserver.h"
#include "Subject.generated.h"

UCLASS()
class CENTCOSMOS_API ASubject : public AActor
{
    GENERATED_BODY()
public:
    void AddObserver(IObserver* Observer);
    void RemoveObserver(IObserver* Observer);
    void NotifyObservers(FName EventType, float Valor);

private:
    TArray<IObserver*> Observers;
};