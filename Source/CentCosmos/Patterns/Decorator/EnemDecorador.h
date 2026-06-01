#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemDecorador.generated.h"

UCLASS()
class CENTCOSMOS_API UEnemDecorador : public UObject
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    AActor* Inner;

    float VidaActual;
    float VidaMaxima;

    UPROPERTY(EditDefaultsOnly, Category = "Decorador")
    float DuracionSobrecargaApex;

    UPROPERTY(EditDefaultsOnly, Category = "Decorador")
    float DuracionDisparoTriple;

public:
    void AplicarVida(AActor* Objetivo, float Cantidad)
    {
        Inner = Objetivo;
        VidaActual = Cantidad;
        VidaMaxima = Cantidad;
    }

    void RegenerarVida(float Bonus)
    {
        VidaActual = FMath::Min(VidaActual + Bonus, VidaMaxima);
    }

    float GetVida() const { return VidaActual; }

    void RecibirDanio(float Cantidad) { VidaActual = FMath::Max(VidaActual - Cantidad, 0.0f); }

    bool EstaVivo() const { return VidaActual > 0.0f; }

    void AplicarSobrecargaApex(class ACentCosmosPawn* Nave);
    void AplicarDisparoTriple(class ACentCosmosPawn* Nave);
};
