    // Fill out your copyright notice in the Description page of Project Settings.


    #include "TribunalBase.h"
#include "CentCosmosGameMode.h"
    #include "Kismet/GameplayStatics.h"
    #include "Engine/Engine.h" 

    ATribunalBase::ATribunalBase()
    {
        PrimaryActorTick.bCanEverTick = false;
        bEstaActivo = false;

        MallaTribunal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaTribunal"));
        RootComponent = MallaTribunal;
    }

    void ATribunalBase::OnConstruction(const FTransform& Transform)
    {
        Super::OnConstruction(Transform);

        UWorld* World = GetWorld();
        if (World)
        {
            TArray<AActor*> Instancias;
            UGameplayStatics::GetAllActorsOfClass(World, GetClass(), Instancias);

            if (Instancias.Num() > 1)
            {
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s Ya existe en el nivel"), *GetClass()->GetName()));
                }

                Destroy();
            }
        }
    }

    void ATribunalBase::RecibirDanioJefe(float Cantidad)
    {
        Vida -= Cantidad;
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
            FString::Printf(TEXT("Tribunal impactado. Vida: %f"), Vida));

        if (Vida <= 0.f)
        {
            ACentCosmosGameMode* GM = Cast<ACentCosmosGameMode>(
                UGameplayStatics::GetGameMode(this));
            if (GM) GM->OnJefeDerrotado(true); // true = jefe muerto, jugador ganó

            Destroy();
        }
    }

