// GestionNivel.h
// Subsistema de gestion de nivel y experiencia del jugador.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GestionNivel.generated.h"

UCLASS(BlueprintType)
class CENTCOSMOS_API UGestionNivel : public UObject
{
    GENERATED_BODY()

public:
    UGestionNivel();

    void Inicializar();
    void Restaurar();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Nivel")
    int32 GetNivelActual() const { return NivelActual; }

    UFUNCTION(BlueprintCallable, Category = "Nivel")
    void IncrementarNivel();

    UFUNCTION(BlueprintCallable, Category = "Nivel")
    void AnyadirExperiencia(int32 Cantidad);

private:
    UPROPERTY(VisibleAnywhere, Category = "Nivel")
    int32 NivelActual;

    UPROPERTY(VisibleAnywhere, Category = "Nivel")
    int32 ExperienciaActual;

    UPROPERTY(EditDefaultsOnly, Category = "Nivel")
    int32 ExperienciaRequerida;

    void RevisarSubidaDeNivel();
};
