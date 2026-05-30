// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilMagma.generated.h"

class ATribunal_Ira;

UCLASS()
class CENTCOSMOS_API AProyectilMagma : public AActor
{
    GENERATED_BODY()

public:
    AProyectilMagma();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UStaticMeshComponent* MallaMagma;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float Velocidad;

    FVector DireccionVuelo;

    ATribunal_Ira* JefeEmisor;

private:
    int32 ContadorRebotes;
    float CronometroRastro;
    float IntervaloRastro;

    void SpawnRastroFuego();
};
