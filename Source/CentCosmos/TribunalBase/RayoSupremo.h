// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RayoSupremo.generated.h"

class UNiagaraComponent; // Declaración adelantada de Niagara

UCLASS()
class CENTCOSMOS_API ARayoSupremo : public AActor
{
    GENERATED_BODY()

public:
    ARayoSupremo();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    class USceneComponent* RaizGrupo;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    class UStaticMeshComponent* MallaAdvertenciaComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    class UStaticMeshComponent* MallaLetalComp;

    // --- NUEVO: Componente del láser visual Supremo ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UNiagaraComponent* EfectoRayoNiagara;

private:
    FTimerHandle TimerFaseLetal;
    bool bEsRayoLetal;

    void ActivarRayoLetal();
};
