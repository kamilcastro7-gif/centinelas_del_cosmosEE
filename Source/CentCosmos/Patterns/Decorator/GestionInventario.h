// GestionInventario.h
// Subsistema de gestion del inventario del jugador.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GestionInventario.generated.h"

UCLASS(BlueprintType)
class CENTCOSMOS_API UGestionInventario : public UObject
{
    GENERATED_BODY()

public:
    UGestionInventario();

    void Inicializar();
    void Limpiar();

    UFUNCTION(BlueprintCallable, Category = "Inventario")
    bool Anyadir(const FName& IdObjeto);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventario")
    bool Contiene(const FName& IdObjeto) const;

private:
    UPROPERTY(VisibleAnywhere, Category = "Inventario")
    TArray<FName> Objetos;
};
