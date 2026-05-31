#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FachadaEstadosJugador.generated.h"

class UGestionVida;
class UGestionStamina;
class UGestionNivel;
class UGestionInventario;

UCLASS()
class CENTCOSMOS_API UFachadaEstadosJugador : public UObject
{
    GENERATED_BODY()

public:
    UFachadaEstadosJugador();

    void IniciarEstadoJugador();
    void ActualizarEstado(float DeltaTime);
    void ReiniciarEstados();

    float ObtenerVida() const;
    void EstablecerVida(float NuevaVida);
    void ModificarVida(float Delta);

    float ObtenerStamina() const;
    void EstablecerStamina(float NuevaStamina);
    bool ConsumirStamina(float Costo);

    int32 ObtenerNivel() const;
    void SubirNivel();
    void AnyadirExperiencia(int32 Cantidad);

    bool AnyadirObjetoAlInventario(const FName& IdObjeto);
    bool TieneObjetoEnInventario(const FName& IdObjeto) const;

    bool EstaVivo() const;

private:
    UPROPERTY()
    UGestionVida* GestionVida;

    UPROPERTY()
    UGestionStamina* GestionStamina;

    UPROPERTY()
    UGestionNivel* GestionNivel;

    UPROPERTY()
    UGestionInventario* GestionInventario;
};