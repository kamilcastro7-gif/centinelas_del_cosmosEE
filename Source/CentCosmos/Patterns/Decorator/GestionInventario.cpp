// GestionInventario.cpp

#include "GestionInventario.h"

UGestionInventario::UGestionInventario()
{
}

void UGestionInventario::Inicializar()
{
    Objetos.Empty();
}

void UGestionInventario::Limpiar()
{
    Objetos.Empty();
}

bool UGestionInventario::Anyadir(const FName& IdObjeto)
{
    if (IdObjeto == NAME_None) return false;
    Objetos.Add(IdObjeto);
    return true;
}

bool UGestionInventario::Contiene(const FName& IdObjeto) const
{
    return Objetos.Contains(IdObjeto);
}
