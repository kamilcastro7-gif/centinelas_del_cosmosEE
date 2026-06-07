// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enjambre.h"
#include "EVigia_Del_Vacio.generated.h"

class AProyectilBase;
class USkeletalMeshComponent; // NUEVO: Para la malla visual del dron

UCLASS()
class CENTCOSMOS_API AEVigia_Del_Vacio : public AEnjambre
{
    GENERATED_BODY()
public:
    AEVigia_Del_Vacio();
    virtual void Tick(float DeltaTime) override;
    void moverVigia();
    void VigAtacar();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Movimiento")
    float VelocidadVigia;

    UPROPERTY(EditAnywhere, Category = "Movimiento")
    float AmplitudPatrulla;

public:
    // --- COMPONENTE VISUAL DEL DRON ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    USkeletalMeshComponent* MallaDrone;

private:
    FVector PosicionInicialSpawn;
};
