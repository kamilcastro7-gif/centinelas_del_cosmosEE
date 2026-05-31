#include "FachadaEstadosJugador.h"
#include "../Decorator/GestionVida.h"
#include "../Decorator/GestionStamina.h"
#include "../Decorator/GestionNivel.h"
#include "../Decorator/GestionInventario.h"

UFachadaEstadosJugador::UFachadaEstadosJugador()
{
    GestionVida       = NewObject<UGestionVida>(this, TEXT("GestionVida"));
    GestionStamina    = NewObject<UGestionStamina>(this, TEXT("GestionStamina"));
    GestionNivel      = NewObject<UGestionNivel>(this, TEXT("GestionNivel"));
    GestionInventario = NewObject<UGestionInventario>(this, TEXT("GestionInventario"));
}

void UFachadaEstadosJugador::IniciarEstadoJugador()
{
    GestionVida->Inicializar();
    GestionStamina->Inicializar();
    GestionNivel->Inicializar();
    GestionInventario->Inicializar();
}

void UFachadaEstadosJugador::ActualizarEstado(float DeltaTime)
{
    GestionStamina->Regenerar(DeltaTime);
    GestionVida->Regenerar(DeltaTime);
}

void UFachadaEstadosJugador::ReiniciarEstados()
{
    GestionVida->Restaurar();
    GestionStamina->Restaurar();
    GestionNivel->Restaurar();
    GestionInventario->Limpiar();
}

float UFachadaEstadosJugador::ObtenerVida() const
{
    return GestionVida->GetVidaActual();
}

void UFachadaEstadosJugador::EstablecerVida(float NuevaVida)
{
    GestionVida->SetVidaActual(NuevaVida);
}

void UFachadaEstadosJugador::ModificarVida(float Delta)
{
    GestionVida->AnyadirVida(Delta);
}

float UFachadaEstadosJugador::ObtenerStamina() const
{
    return GestionStamina->GetStaminaActual();
}

void UFachadaEstadosJugador::EstablecerStamina(float NuevaStamina)
{
    GestionStamina->SetStaminaActual(NuevaStamina);
}

bool UFachadaEstadosJugador::ConsumirStamina(float Costo)
{
    return GestionStamina->Consumir(Costo);
}

int32 UFachadaEstadosJugador::ObtenerNivel() const
{
    return GestionNivel->GetNivelActual();
}

void UFachadaEstadosJugador::SubirNivel()
{
    GestionNivel->IncrementarNivel();
}

void UFachadaEstadosJugador::AnyadirExperiencia(int32 Cantidad)
{
    GestionNivel->AnyadirExperiencia(Cantidad);
}

bool UFachadaEstadosJugador::AnyadirObjetoAlInventario(const FName& IdObjeto)
{
    return GestionInventario->Anyadir(IdObjeto);
}

bool UFachadaEstadosJugador::TieneObjetoEnInventario(const FName& IdObjeto) const
{
    return GestionInventario->Contiene(IdObjeto);
}

bool UFachadaEstadosJugador::EstaVivo() const
{
    return GestionVida->GetVidaActual() > 0.0f;
}