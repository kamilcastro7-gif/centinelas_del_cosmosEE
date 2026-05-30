// FachadaEstadosJugador.h
// Fachada que centraliza el acceso a los subsistemas de estado del jugador.
// Patron de diseno: Facade

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FachadaEstadosJugador.generated.h"

// -----------------------------------------------------------------------------
// Forward declarations de los subsistemas gestionados por la fachada
// -----------------------------------------------------------------------------
class UGestionVida;
class UGestionStamina;
class UGestionNivel;
class UGestionInventario;

/**
 * Fachada que unifica la interaccion con los subsistemas de estado del jugador.
 * Proporciona una interfaz simplificada para consultar y modificar vida,
 * stamina, nivel, inventario y otros estados del jugador.
 */
UCLASS(Blueprintable, BlueprintType)
class CENTCOSMOS_API UFachadaEstadosJugador : public UObject
{
    GENERATED_BODY()

public:
    // Constructor
    UFachadaEstadosJugador();

    // -------------------------------------------------------------------------
    // Inicializacion y actualizacion
    // -------------------------------------------------------------------------

    /** Inicializa todos los subsistemas de estado del jugador. */
    UFUNCTION(BlueprintCallable, Category = "Fachada|Inicializacion")
    void IniciarEstadoJugador();

    /** Actualiza el estado de todos los subsistemas cada frame. */
    UFUNCTION(BlueprintCallable, Category = "Fachada|Actualizacion")
    void ActualizarEstado(float DeltaTime);

    /** Reinicia todos los estados a sus valores por defecto. */
    UFUNCTION(BlueprintCallable, Category = "Fachada|Reinicio")
    void ReiniciarEstados();

    // -------------------------------------------------------------------------
    // Metodos de vida
    // -------------------------------------------------------------------------

    /** Devuelve la vida actual del jugador. */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fachada|Vida")
    float ObtenerVida() const;

    /** Establece la vida actual del jugador. */
    UFUNCTION(BlueprintCallable, Category = "Fachada|Vida")
    void EstablecerVida(float NuevaVida);

    /** Anyade o resta vida al jugador. */
    UFUNCTION(BlueprintCallable, Category = "Fachada|Vida")
    void ModificarVida(float Delta);

    // -------------------------------------------------------------------------
    // Metodos de stamina
    // -------------------------------------------------------------------------

    /** Devuelve la stamina actual del jugador. */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fachada|Stamina")
    float ObtenerStamina() const;

    /** Establece la stamina actual del jugador. */
    UFUNCTION(BlueprintCallable, Category = "Fachada|Stamina")
    void EstablecerStamina(float NuevaStamina);

    /** Consume stamina al realizar acciones. */
    UFUNCTION(BlueprintCallable, Category = "Fachada|Stamina")
    bool ConsumirStamina(float Costo);

    // -------------------------------------------------------------------------
    // Metodos de nivel
    // -------------------------------------------------------------------------

    /** Devuelve el nivel actual del jugador. */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fachada|Nivel")
    int32 ObtenerNivel() const;

    /** Incrementa el nivel del jugador en uno. */
    UFUNCTION(BlueprintCallable, Category = "Fachada|Nivel")
    void SubirNivel();

    /** Anyade experiencia al jugador y sube de nivel si corresponde. */
    UFUNCTION(BlueprintCallable, Category = "Fachada|Nivel")
    void AnyadirExperiencia(int32 Cantidad);

    // -------------------------------------------------------------------------
    // Metodos de inventario
    // -------------------------------------------------------------------------

    /** Anyade un objeto al inventario del jugador. */
    UFUNCTION(BlueprintCallable, Category = "Fachada|Inventario")
    bool AnyadirObjetoAlInventario(const FName& IdObjeto);

    /** Verifica si el jugador posee un objeto en el inventario. */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fachada|Inventario")
    bool TieneObjetoEnInventario(const FName& IdObjeto) const;

    // -------------------------------------------------------------------------
    // Metodo de utilidad general
    // -------------------------------------------------------------------------

    /** Devuelve true si el jugador esta vivo (vida > 0). */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fachada|Estado")
    bool EstaVivo() const;

private:
    // Subsistemas gestionados por la fachada
    UPROPERTY(VisibleAnywhere, Category = "Fachada|Subsistemas")
    UGestionVida* GestionVida;

    UPROPERTY(VisibleAnywhere, Category = "Fachada|Subsistemas")
    UGestionStamina* GestionStamina;

    UPROPERTY(VisibleAnywhere, Category = "Fachada|Subsistemas")
    UGestionNivel* GestionNivel;

    UPROPERTY(VisibleAnywhere, Category = "Fachada|Subsistemas")
    UGestionInventario* GestionInventario;
};
