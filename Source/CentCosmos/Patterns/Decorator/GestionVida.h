// GestionVida.h
// Subsistema de gestion de vida del jugador.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GestionVida.generated.h"

UCLASS(BlueprintType)
class CENTCOSMOS_API UGestionVida : public UObject
{
    GENERATED_BODY()

public:
    UGestionVida();

    void Inicializar();
    void Regenerar(float DeltaTime);
    void Restaurar();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Vida")
    float GetVidaActual() const { return VidaActual; }

    UFUNCTION(BlueprintCallable, Category = "Vida")
    void SetVidaActual(float NuevaVida) { VidaActual = FMath::Clamp(NuevaVida, 0.0f, VidaMaxima); }

    UFUNCTION(BlueprintCallable, Category = "Vida")
    void AnyadirVida(float Delta) { SetVidaActual(VidaActual + Delta); }

private:
    UPROPERTY(VisibleAnywhere, Category = "Vida")
    float VidaActual;

    UPROPERTY(EditDefaultsOnly, Category = "Vida")
    float VidaMaxima;

    UPROPERTY(EditDefaultsOnly, Category = "Vida")
    float RegeneracionPorSegundo;
};
