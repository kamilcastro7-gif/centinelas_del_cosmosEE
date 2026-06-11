// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enjambre.h"
#include "EHeraldo_De_La_Ruina.generated.h"

class AProyectilBase;
class USkeletalMeshComponent; // NUEVO: Para la malla del Heraldo

UCLASS()
class CENTCOSMOS_API AEHeraldo_De_La_Ruina : public AEnjambre
{
    GENERATED_BODY()
public:
    AEHeraldo_De_La_Ruina();

    // Añadimos el Tick para que moverHeraldo() se ejecute
    virtual void Tick(float DeltaTime) override;

    void moverHeraldo();
    void HerAtacar();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Combate")
    float VelocidadHeraldo;

public:
    // --- COMPONENTE VISUAL DEL DRON ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    USkeletalMeshComponent* MallaHeraldo;

private:
    float TimerEstadoHeraldo;
    bool bEstaPersiguiendo;
};
