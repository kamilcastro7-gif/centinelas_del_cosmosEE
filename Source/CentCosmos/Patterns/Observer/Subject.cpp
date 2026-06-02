#include "Subject.h"

void ASubject::AddObserver(IObserver* Observer)
{
    if (Observer && !Observers.Contains(Observer))
        Observers.Add(Observer);
}

void ASubject::RemoveObserver(IObserver* Observer)
{
    if (Observer)
        Observers.Remove(Observer);
}

void ASubject::NotifyObservers(FName EventType, float Valor)
{
    for (IObserver* Observer : Observers)
    {
        if (Observer)
            Observer->OnNotify(EventType, Valor);
    }
}