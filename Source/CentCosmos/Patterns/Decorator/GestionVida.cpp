// GestionVida.cpp

#include "GestionVida.h"

UGestionVida::UGestionVida()
{
    VidaActual = 100.0f;
    VidaMaxima = 100.0f;
    RegeneracionPorSegundo = 0.0f;
}

void UGestionVida::Inicializar()
{
    VidaActual = VidaMaxima;
}

void UGestionVida::Regenerar(float DeltaTime)
{
    if (RegeneracionPorSegundo > 0.0f && VidaActual < VidaMaxima)
    {
        VidaActual = FMath::Min(VidaActual + RegeneracionPorSegundo * DeltaTime, VidaMaxima);
    }
}

void UGestionVida::Restaurar()
{
    VidaActual = VidaMaxima;
}
