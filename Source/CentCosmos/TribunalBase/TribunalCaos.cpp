// Fill out your copyright notice in the Description page of Project Settings.

#include "TribunalCaos.h"
#include "HologramaCaos.h"
#include "ProyectilCaos.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ATribunalCaos::ATribunalCaos()
{
    PrimaryActorTick.bCanEverTick = true;
    bEstaActivo = false;
    PasoDisparoActual = 1;
    RangoDeteccionManual = 2200.f;
    Vida = 180.0f;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Wedge_B.Shape_Wedge_B'"));
    if (MallaTribunal)
    {
        MallaTribunal->SetCollisionProfileName(TEXT("BlockAll"));
        MallaTribunal->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

        if (MeshAsset.Succeeded())
        {
            MallaTribunal->SetStaticMesh(MeshAsset.Object);
        }

        MallaTribunal->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
    }
}

void ATribunalCaos::BeginPlay()
{
    Super::BeginPlay();

    FVector Origen = GetActorLocation();

    PosicionesFijas.Empty();
    PosicionesFijas.Add(Origen + FVector(0.f, -800.f, 0.f));
    PosicionesFijas.Add(Origen);
    PosicionesFijas.Add(Origen + FVector(0.f, 800.f, 0.f));

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    Holograma1 = GetWorld()->SpawnActor<AHologramaCaos>(AHologramaCaos::StaticClass(), PosicionesFijas[0], FRotator::ZeroRotator, SpawnParams);
    Holograma2 = GetWorld()->SpawnActor<AHologramaCaos>(AHologramaCaos::StaticClass(), PosicionesFijas[2], FRotator::ZeroRotator, SpawnParams);

    DesactivarJefe();
}

void ATribunalCaos::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    APawn* NaveJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (NaveJugador)
    {
        float DistanciaAlJugador = FVector::Dist2D(GetActorLocation(), NaveJugador->GetActorLocation());

        if (DistanciaAlJugador <= RangoDeteccionManual)
        {
            if (!bEstaActivo)
            {
                ActivarJefe();
            }
        }
        else
        {
            if (bEstaActivo)
            {
                DesactivarJefe();
            }
        }
    }
}

void ATribunalCaos::ActivarJefe()
{
    Super::ActivarJefe();
    bEstaActivo = true;
    ReubicarActoresAlAzar();
    IniciarFaseVisible();
}

void ATribunalCaos::DesactivarJefe()
{
    bEstaActivo = false;

    GetWorld()->GetTimerManager().ClearTimer(TimerCicloVisibilidad);
    GetWorld()->GetTimerManager().ClearTimer(TimerDisparoSecuencial);

    SetActorHiddenInGame(true);
    if (MallaTribunal) MallaTribunal->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (Holograma1) Holograma1->SetActorHiddenInGame(true);
    if (Holograma2) Holograma2->SetActorHiddenInGame(true);

    if (PosicionesFijas.Num() > 1)
    {
        SetActorLocation(PosicionesFijas[1]);
    }
}

void ATribunalCaos::IniciarFaseVisible()
{
    if (!bEstaActivo) return;

    SetActorHiddenInGame(false);
    if (MallaTribunal) MallaTribunal->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    if (Holograma1) Holograma1->SetActorHiddenInGame(false);
    if (Holograma2) Holograma2->SetActorHiddenInGame(false);

    PasoDisparoActual = 1;

    GetWorld()->GetTimerManager().SetTimer(TimerDisparoSecuencial, this, &ATribunalCaos::EjecutarDisparoEnOrden, 1.8f, true, 1.0f);
    GetWorld()->GetTimerManager().SetTimer(TimerCicloVisibilidad, this, &ATribunalCaos::IniciarFaseOculta, 7.0f, false);
}

void ATribunalCaos::IniciarFaseOculta()
{
    if (!bEstaActivo) return;

    GetWorld()->GetTimerManager().ClearTimer(TimerDisparoSecuencial);

    SetActorHiddenInGame(true);
    if (MallaTribunal) MallaTribunal->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (Holograma1) Holograma1->SetActorHiddenInGame(true);
    if (Holograma2) Holograma2->SetActorHiddenInGame(true);

    ReubicarActoresAlAzar();

    GetWorld()->GetTimerManager().SetTimer(TimerCicloVisibilidad, this, &ATribunalCaos::IniciarFaseVisible, 2.0f, false);
}

void ATribunalCaos::ReubicarActoresAlAzar()
{
    TArray<AActor*> Actores;
    Actores.Add(this);
    if (Holograma1) Actores.Add(Holograma1);
    if (Holograma2) Actores.Add(Holograma2);

    int32 UltimoIndice = Actores.Num() - 1;
    for (int32 i = 0; i <= UltimoIndice; ++i)
    {
        int32 IndiceAzar = FMath::RandRange(i, UltimoIndice);
        if (i != IndiceAzar)
        {
            Actores.Swap(i, IndiceAzar);
        }
    }

    for (int32 i = 0; i < Actores.Num(); ++i)
    {
        if (Actores[i] && PosicionesFijas.IsValidIndex(i))
        {
            Actores[i]->SetActorLocation(PosicionesFijas[i]);
        }
    }
}

void ATribunalCaos::EjecutarDisparoEnOrden()
{
    if (!bEstaActivo) return;

    FVector PuestoBuscado = FVector::ZeroVector;

    if (PasoDisparoActual >= 1 && PasoDisparoActual <= 3)
    {
        PuestoBuscado = PosicionesFijas[PasoDisparoActual - 1];
    }

    AActor* ActorEnPuesto = nullptr;

    if (GetActorLocation().Equals(PuestoBuscado, 15.f)) ActorEnPuesto = this;
    else if (Holograma1 && Holograma1->GetActorLocation().Equals(PuestoBuscado, 15.f)) ActorEnPuesto = Holograma1;
    else if (Holograma2 && Holograma2->GetActorLocation().Equals(PuestoBuscado, 15.f)) ActorEnPuesto = Holograma2;

    if (ActorEnPuesto)
    {
        if (ActorEnPuesto == this)
        {
            APawn* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
            if (Jugador)
            {
                FVector Dir = (Jugador->GetActorLocation() - GetActorLocation());
                Dir.Z = 0.f; Dir.Normalize();

                FActorSpawnParameters SpawnParams;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                GetWorld()->SpawnActor<AProyectilCaos>(AProyectilCaos::StaticClass(), GetActorLocation() + (Dir * 150.f), Dir.Rotation(), SpawnParams);
            }
        }
        else
        {
            AHologramaCaos* Holo = Cast<AHologramaCaos>(ActorEnPuesto);
            if (Holo) Holo->Disparar();
        }
    }

    PasoDisparoActual++;
    if (PasoDisparoActual > 3)
    {
        PasoDisparoActual = 1;
    }
}

void ATribunalCaos::Destroyed()
{
    // 1. Destruimos los hologramas si es que existen en el nivel
    if (Holograma1)
    {
        Holograma1->Destroy();
    }

    if (Holograma2)
    {
        Holograma2->Destroy();
    }

    // 2. Llamamos a la función original del motor para terminar de borrar al Jefe
    Super::Destroyed();
}