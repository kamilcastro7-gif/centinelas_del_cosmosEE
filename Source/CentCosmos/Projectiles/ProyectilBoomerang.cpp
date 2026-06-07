// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilBoomerang.h"
#include "Components/StaticMeshComponent.h"
#include "CentCosmosPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "ProyectilBase.h" 
#include "Enjambre.h" 
#include "../EExclusivo.h"
#include "TribunalBase/TribunalBase.h"
#include "Escudo.h"
#include "TribunalBase/PlacaMetal.h"
// --- NUEVOS INCLUDES PARA NIAGARA ---
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AProyectilBoomerang::AProyectilBoomerang()
{
    PrimaryActorTick.bCanEverTick = true;

    // 1. HITBOX INVISIBLE: Torus base
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TorusMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus'"));
    ProyectilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoomerangMesh"));
    RootComponent = ProyectilMesh;

    if (TorusMesh.Succeeded()) ProyectilMesh->SetStaticMesh(TorusMesh.Object);

    // Ocultamos la malla visualmente
    ProyectilMesh->SetHiddenInGame(true);

    // --- CORRECCIÓN 1: Colisión absoluta de superposición para que atraviese todo ---
    ProyectilMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ProyectilMesh->SetCollisionResponseToAllChannels(ECR_Overlap);

    ProyectilMesh->OnComponentBeginOverlap.AddDynamic(this, &AProyectilBoomerang::OnOverlap);
    ProyectilMesh->SetRelativeScale3D(FVector(1.8f, 1.8f, 1.8f));

    // 2. EFECTO VISUAL NIAGARA
    EfectoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoNiagara"));
    EfectoNiagara->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Energy_4.NS_Energy_4'"));
    if (NiagaraAsset.Succeeded())
    {
        EfectoNiagara->SetAsset(NiagaraAsset.Object);
    }

    Velocidad = 2000.f;
    TiempoDeGiro = 1.2f;
    TiempoEnVuelo = 0.f;
    bRegresando = false;

    Danio = 5.0f;
}

void AProyectilBoomerang::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TiempoEnVuelo += DeltaTime;

    if (!bRegresando && TiempoEnVuelo >= TiempoDeGiro) bRegresando = true;

    float VelocidadActual = Velocidad;
    if (NaveDueno && NaveDueno->bRalentizadoPorChispa)
    {
        VelocidadActual = Velocidad * 0.4f;
    }

    if (!bRegresando)
    {
        FVector NuevaPosicion = GetActorLocation() + (GetActorForwardVector() * VelocidadActual * DeltaTime);
        SetActorLocation(NuevaPosicion);
    }
    else if (NaveDueno)
    {
        FVector DireccionALaNave = (NaveDueno->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        FVector NuevaPosicion = GetActorLocation() + (DireccionALaNave * VelocidadActual * DeltaTime);

        AddActorLocalRotation(FRotator(0.f, 5.f, 0.f));
        SetActorLocation(NuevaPosicion);
    }
}

void AProyectilBoomerang::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        AEnjambre* EnemigoImpactado = Cast<AEnjambre>(OtherActor);
        if (EnemigoImpactado) { EnemigoImpactado->RecibirDanioEnemigo(Danio); }

        AEExclusivo* ExclusivoImpactado = Cast<AEExclusivo>(OtherActor);
        if (ExclusivoImpactado) { ExclusivoImpactado->RecibirDanoEnemigo(Danio); }

        ATribunalBase* Tribunal = Cast<ATribunalBase>(OtherActor);
        if (Tribunal) { Tribunal->RecibirDanioJefe(Danio); }

        APlacaMetal* Placa = Cast<APlacaMetal>(OtherActor);
        if (Placa) { Placa->RecibirDanioPlaca(Danio); }

        AEscudo* Escudo = Cast<AEscudo>(OtherActor);
        if (Escudo) Escudo->RecibirDanoEscudo(Danio);

        // --- CORRECCIÓN 2: Destruir el boomerang cuando vuelve a tus manos ---
        if (bRegresando && OtherActor == NaveDueno)
        {
            if (NaveDueno) NaveDueno->bBoomerangEnVuelo = false;
            Destroy();
        }
    }
}

