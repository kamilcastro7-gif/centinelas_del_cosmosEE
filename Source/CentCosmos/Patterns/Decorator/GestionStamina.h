// GestionStamina.h
// Subsistema de gestion de stamina del jugador.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GestionStamina.generated.h"

UCLASS(BlueprintType)
class CENTCOSMOS_API UGestionStamina : public UObject
{
    GENERATED_BODY()

public:
    UGestionStamina();

    void Inicializar();
    void Regenerar(float DeltaTime);
    void Restaurar();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stamina")
    float GetStaminaActual() const { return StaminaActual; }

    UFUNCTION(BlueprintCallable, Category = "Stamina")
    void SetStaminaActual(float NuevaStamina) { StaminaActual = FMath::Clamp(NuevaStamina, 0.0f, StaminaMaxima); }

    UFUNCTION(BlueprintCallable, Category = "Stamina")
    bool Consumir(float Costo);

private:
    UPROPERTY(VisibleAnywhere, Category = "Stamina")
    float StaminaActual;

    UPROPERTY(EditDefaultsOnly, Category = "Stamina")
    float StaminaMaxima;

    UPROPERTY(EditDefaultsOnly, Category = "Stamina")
    float RegeneracionPorSegundo;
};
