#include "VidaObserver.h"

UVidaObserver::UVidaObserver()
{
    UmbralVidaCritica = 25.0f;
}

void UVidaObserver::OnNotify(FName EventType)
{
    if (EventType == "VidaActualizada")
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Vida actualizada"));
    }
}