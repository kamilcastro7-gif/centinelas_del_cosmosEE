#include "VidaObserver.h"

void UVidaObserver::OnNotify(FName EventType, float Valor)
{
    if (EventType == FName("VidaActualizada"))
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(
                1, 3.f, FColor::Cyan,
                FString::Printf(TEXT("Vida: %.0f"), Valor));
    }
}