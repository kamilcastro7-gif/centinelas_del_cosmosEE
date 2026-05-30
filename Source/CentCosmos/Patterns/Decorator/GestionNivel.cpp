// GestionNivel.cpp

#include "GestionNivel.h"

UGestionNivel::UGestionNivel()
{
    NivelActual = 1;
    ExperienciaActual = 0;
    ExperienciaRequerida = 100;
}

void UGestionNivel::Inicializar()
{
    NivelActual = 1;
    ExperienciaActual = 0;
}

void UGestionNivel::Restaurar()
{
    Inicializar();
}

void UGestionNivel::IncrementarNivel()
{
    NivelActual++;
}

void UGestionNivel::AnyadirExperiencia(int32 Cantidad)
{
    ExperienciaActual += Cantidad;
    RevisarSubidaDeNivel();
}

void UGestionNivel::RevisarSubidaDeNivel()
{
    while (ExperienciaActual >= ExperienciaRequerida)
    {
        ExperienciaActual -= ExperienciaRequerida;
        IncrementarNivel();
    }
}
