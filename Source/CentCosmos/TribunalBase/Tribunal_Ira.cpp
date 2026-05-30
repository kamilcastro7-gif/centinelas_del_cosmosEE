// Fill out your copyright notice in the Description page of Project Settings.

#include "Tribunal_Ira.h"
#include "ProyectilMagma.h"
#include "PFuego.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ATribunal_Ira::ATribunal_Ira()
{
    PrimaryActorTick.bCanEverTick = true;
    bEstaActivo = false;
    MagmaActivoEnPantalla = 0;

    CronometroPFuego = 0.f;
    CadenciaPFuego = 5.0f;

    VelocidadDesplazamiento = 250.f;
    RadioMaximoMovimiento = 500.f;
    TiempoEsperaEnPunto = 1.5f;
    RangoDeteccionManual = 2200.f;
    bPuedeMoverse = true;

    ClaseMagma = AProyectilMagma::StaticClass();

    if (MallaTribunal)
    {
        MallaTribunal->SetMobility(EComponentMobility::Movable);
        static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Wedge_A.Shape_Wedge_A'"));
        if (MeshAsset.Succeeded())
        {
            MallaTribunal->SetStaticMesh(MeshAsset.Object);
            MallaTribunal->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
            MallaTribunal->SetCollisionProfileName(TEXT("BlockAll"));
        }
    }
}

void ATribunal_Ira::BeginPlay()
{
    Super::BeginPlay();
    SetActorTickEnabled(true);
    PosicionInicialAnclaje = GetActorLocation();
    PuntoDestinoActual = PosicionInicialAnclaje;
}

void ATribunal_Ira::Tick(float DeltaTime)
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
                bEstaActivo = false;
                bPuedeMoverse = false;
                PuntoDestinoActual = PosicionInicialAnclaje;
                GetWorld()->GetTimerManager().ClearTimer(TimerEsperaMovimiento);
            }
        }
    }

    if (!bEstaActivo)
    {
        FVector PosicionActual = GetActorLocation();
        if (!PosicionActual.Equals(PosicionInicialAnclaje, 5.f))
        {
            FVector RegresoSuave = FMath::VInterpTo(PosicionActual, PosicionInicialAnclaje, DeltaTime, 2.0f);
            SetActorLocation(RegresoSuave);
        }
        return;
    }

    FVector PosicionActual = GetActorLocation();
    if (bPuedeMoverse && !PosicionActual.Equals(PuntoDestinoActual, 10.f))
    {
        FVector NuevaPosicion = FMath::VInterpTo(PosicionActual, PuntoDestinoActual, DeltaTime, VelocidadDesplazamiento / 100.f);
        NuevaPosicion.Z = PosicionInicialAnclaje.Z;
        SetActorLocation(NuevaPosicion);
    }
    else if (bPuedeMoverse)
    {
        bPuedeMoverse = false;
        GetWorld()->GetTimerManager().SetTimer(TimerEsperaMovimiento, this, &ATribunal_Ira::CalcularNuevoPuntoAleatorio, TiempoEsperaEnPunto, false);
    }

    CronometroPFuego += DeltaTime;
    if (CronometroPFuego >= CadenciaPFuego)
    {
        DispararPFuegoConstante();
        CronometroPFuego = 0.f;
    }

    if (MagmaActivoEnPantalla == 0)
    {
        IntentarDispararMagma();
    }
}

void ATribunal_Ira::ActivarJefe()
{
    Super::ActivarJefe();

    bPuedeMoverse = true;
    PuntoDestinoActual = GetActorLocation();
    CronometroPFuego = 0.f;
    UE_LOG(LogTemp, Warning, TEXT("Tribunal_Ira: Sistemas de combate encendidos limpiamente."));
}

void ATribunal_Ira::CalcularNuevoPuntoAleatorio()
{
    if (!bEstaActivo) return;

    float DesfaseX = FMath::FRandRange(-RadioMaximoMovimiento, RadioMaximoMovimiento);
    float DesfaseY = FMath::FRandRange(-RadioMaximoMovimiento, RadioMaximoMovimiento);

    PuntoDestinoActual = PosicionInicialAnclaje + FVector(DesfaseX, DesfaseY, 0.f);
    bPuedeMoverse = true;
}

void ATribunal_Ira::IntentarDispararMagma()
{
    APawn* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Jugador || !ClaseMagma) return;

    FVector MiPosicion = GetActorLocation();
    FVector DireccionHaciaJugador = (Jugador->GetActorLocation() - MiPosicion);
    DireccionHaciaJugador.Z = 0.f;
    DireccionHaciaJugador.Normalize();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    float Desfases[2] = { -15.f, 15.f };

    for (int32 i = 0; i < 2; i++)
    {
        FRotator RotacionSpawn = DireccionHaciaJugador.Rotation();
        RotacionSpawn.Yaw += Desfases[i];

        FVector DireccionFinal = RotacionSpawn.Vector();
        DireccionFinal.Z = 0.f;
        DireccionFinal.Normalize();

        FVector PuntoSpawn = MiPosicion + (DireccionFinal * 500.f);
        PuntoSpawn.Z = MiPosicion.Z;

        AProyectilMagma* MagmaBox = GetWorld()->SpawnActor<AProyectilMagma>(ClaseMagma, PuntoSpawn, RotacionSpawn, SpawnParams);
        if (MagmaBox)
        {
            MagmaBox->DireccionVuelo = DireccionFinal;
            MagmaBox->JefeEmisor = this;
            MagmaActivoEnPantalla++;
        }
    }
}

void ATribunal_Ira::DispararPFuegoConstante()
{
    APawn* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Jugador) return;

    FVector MiPosicion = GetActorLocation();
    FVector Direccion = (Jugador->GetActorLocation() - MiPosicion);
    Direccion.Z = 0.f;
    Direccion.Normalize();

    FVector PuntoSpawn = MiPosicion + (Direccion * 400.f);
    PuntoSpawn.Z = MiPosicion.Z;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    APFuego* ProyectilChico = GetWorld()->SpawnActor<APFuego>(APFuego::StaticClass(), PuntoSpawn, Direccion.Rotation(), SpawnParams);
    if (ProyectilChico)
    {
        ProyectilChico->DireccionVuelo = Direccion;
    }
}

void ATribunal_Ira::NotificarMuerteMagma()
{
    if (MagmaActivoEnPantalla > 0)
    {
        MagmaActivoEnPantalla--;
    }
}
