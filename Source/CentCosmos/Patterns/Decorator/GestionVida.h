#pragma once

#include "CoreMinimal.h"
#include "DecoratorBase.h"
#include "GestionVida.generated.h"

UCLASS()
class CENTCOSMOS_API UGestionVida : public UDecoratorBase
{
    GENERATED_BODY()

public:
    UGestionVida();

    virtual void Aplicar() override;
    void Inicializar();
    void Regenerar(float DeltaTime);
    void Restaurar();

    float GetVidaActual() const { return VidaActual; }
    float GetVidaMaxima() const { return VidaMaxima; }
    void SetVidaActual(float NuevaVida) { VidaActual = FMath::Clamp(NuevaVida, 0.0f, VidaMaxima); }
    void AnyadirVida(float Delta) { SetVidaActual(VidaActual + Delta); }

private:
    UPROPERTY()
    float VidaActual;

    UPROPERTY()
    float VidaMaxima;

    UPROPERTY()
    float RegeneracionPorSegundo;
};
