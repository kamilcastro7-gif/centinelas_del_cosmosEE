// FachadaEstadosJugador.cpp
// Implementacion de la fachada de estados del jugador.

#include "FachadaEstadosJugador.h"
#include "../Decorator/GestionVida.h"
#include "../Decorator/GestionStamina.h"
#include "../Decorator/GestionNivel.h"
#include "../Decorator/GestionInventario.h"

// -----------------------------------------------------------------------------
// Constructor: crea los subsistemas internos que la fachada va a gestionar.
// -----------------------------------------------------------------------------
UFachadaEstadosJugador::UFachadaEstadosJugador()
{
    GestionVida       = NewObject<UGestionVida>(this, TEXT("GestionVida"));
    GestionStamina    = NewObject<UGestionStamina>(this, TEXT("GestionStamina"));
    GestionNivel      = NewObject<UGestionNivel>(this, TEXT("GestionNivel"));
    GestionInventario = NewObject<UGestionInventario>(this, TEXT("GestionInventario"));
}

// -----------------------------------------------------------------------------
// Inicializacion: delega la configuracion inicial a cada subsistema.
// -----------------------------------------------------------------------------
void UFachadaEstadosJugador::IniciarEstadoJugador()
{
    GestionVida->Inicializar();
    GestionStamina->Inicializar();
    GestionNivel->Inicializar();
    GestionInventario->Inicializar();
}

// -----------------------------------------------------------------------------
// Actualizacion: propaga el DeltaTime a los subsistemas que lo necesiten.
// -----------------------------------------------------------------------------
void UFachadaEstadosJugador::ActualizarEstado(float DeltaTime)
{
    GestionStamina->Regenerar(DeltaTime);
    GestionVida->Regenerar(DeltaTime);
}

// -----------------------------------------------------------------------------
// Reinicio: restaura todos los valores por defecto.
// -----------------------------------------------------------------------------
void UFachadaEstadosJugador::ReiniciarEstados()
{
    GestionVida->Restaurar();
    GestionStamina->Restaurar();
    GestionNivel->Restaurar();
    GestionInventario->Limpiar();
}

// -----------------------------------------------------------------------------
// Metodos de vida
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Metodos de stamina
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Metodos de nivel
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Metodos de inventario
// -----------------------------------------------------------------------------
bool UFachadaEstadosJugador::AnyadirObjetoAlInventario(const FName& IdObjeto)
{
    return GestionInventario->Anyadir(IdObjeto);
}

bool UFachadaEstadosJugador::TieneObjetoEnInventario(const FName& IdObjeto) const
{
    return GestionInventario->Contiene(IdObjeto);
}

// -----------------------------------------------------------------------------
// Utilidad general
// -----------------------------------------------------------------------------
bool UFachadaEstadosJugador::EstaVivo() const
{
    return GestionVida->GetVidaActual() > 0.0f;
}
