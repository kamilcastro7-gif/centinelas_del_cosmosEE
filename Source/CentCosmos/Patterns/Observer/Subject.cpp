#include "Subject.h"

void ASubject::AddObserver(IObserver* Observer)
{
    if (!Observer) return;

    TScriptInterface<IObserver> Entry;
    Entry.SetObject(Cast<UObject>(Observer));
    Entry.SetInterface(Observer);

    if (!Observers.Contains(Entry))
        Observers.Add(Entry);
}

void ASubject::RemoveObserver(IObserver* Observer)
{
    if (!Observer) return;

    TScriptInterface<IObserver> Entry;
    Entry.SetObject(Cast<UObject>(Observer));
    Entry.SetInterface(Observer);

    Observers.Remove(Entry);
}

void ASubject::NotifyObservers(FName EventType, float Valor)
{
    for (TScriptInterface<IObserver>& Obs : Observers)
    {
        if (Obs)
            Obs->OnNotify(EventType, Valor);
    }
}
