#pragma once

#include "CoreMinimal.h"

struct CENTCOSMOS_API FNivel
{
    FString Nombre;
    TArray<TWeakObjectPtr<AActor>> Enemigos;
    float Dificultad;
    float TiempoLimite;

    FNivel() : Dificultad(0.0f), TiempoLimite(0.0f) {}
};