#pragma once

#include "CoreMinimal.h"
#include "DecoratorBase.h"
#include "GestionStamina.generated.h"

UCLASS()
class CENTCOSMOS_API UGestionStamina : public UDecoratorBase
{
    GENERATED_BODY()

public:
    UGestionStamina();

    virtual void Aplicar() override;
    void Inicializar();
    void Regenerar(float DeltaTime);
    void Restaurar();

    float GetStaminaActual() const { return StaminaActual; }
    float GetStaminaMaxima() const { return StaminaMaxima; }
    void SetStaminaActual(float NuevaStamina) { StaminaActual = FMath::Clamp(NuevaStamina, 0.0f, StaminaMaxima); }
    bool Consumir(float Costo);

private:
    UPROPERTY()
    float StaminaActual;

    UPROPERTY()
    float StaminaMaxima;

    UPROPERTY()
    float RegeneracionPorSegundo;
};
