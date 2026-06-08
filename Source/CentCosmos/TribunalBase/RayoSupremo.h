// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RayoSupremo.generated.h"

class UNiagaraComponent;

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
    USceneComponent* RaizGrupo;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UStaticMeshComponent* MallaAdvertenciaComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UStaticMeshComponent* MallaLetalComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UNiagaraComponent* EfectoRayoNiagara;

    UFUNCTION()
    void ActivarRayoLetal();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materiales")
    UMaterialInterface* MaterialAdvertencia;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materiales")
    UMaterialInterface* MaterialLetal;

private:
    FTimerHandle TimerFaseLetal;
    bool bEsRayoLetal;
};
