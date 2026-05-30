// Fill out your copyright notice in the Description page of Project Settings.

#include "TribunalTormenta.h"
#include "ProyectilPlasma.h"
#include "ProyectilRayo.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ATribunalTormenta::ATribunalTormenta()
{
    PrimaryActorTick.bCanEverTick = true;
    bEstaActivo = false;

    MiMalla = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MiMallaTormenta"));
    RootComponent = MiMalla;
    MiMalla->SetMobility(EComponentMobility::Movable);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
    if (MeshAsset.Succeeded())
    {
        MiMalla->SetStaticMesh(MeshAsset.Object);
        MiMalla->SetRelativeScale3D(FVector(2.f, 2.f, 3.f));
        MiMalla->SetCollisionProfileName(TEXT("BlockAll"));
    }

    ClasePlasma = AProyectilPlasma::StaticClass();
    ClaseRayo = AProyectilRayo::StaticClass();

    VelocidadDesplazamiento = 250.f;
    RadioMaximoMovimiento = 500.f;
    TiempoEsperaEnPunto = 1.5f;
    RangoDeteccionManual = 2200.f;
    bPuedeMoverse = true;

    CadenciaRayo = 2.0f;
    CadenciaPlasma = 4.0f;
    CronometroRayo = 0.f;
    CronometroPlasma = 0.f;
}

void ATribunalTormenta::BeginPlay()
{
    Super::BeginPlay();
    PosicionInicialAnclaje = GetActorLocation();
    PuntoDestinoActual = PosicionInicialAnclaje;
}

void ATribunalTormenta::Tick(float DeltaTime)
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

    if (bEstaActivo)
    {
        CronometroRayo += DeltaTime;
        CronometroPlasma += DeltaTime;

        if (CronometroRayo >= CadenciaRayo)
        {
            DispararRayo();
            CronometroRayo = 0.f;
        }

        if (CronometroPlasma >= CadenciaPlasma)
        {
            SpawnPlasma();
            CronometroPlasma = 0.f;
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
            GetWorld()->GetTimerManager().SetTimer(TimerEsperaMovimiento, this, &ATribunalTormenta::CalcularNuevoPuntoAleatorio, TiempoEsperaEnPunto, false);
        }
    }
    else
    {
        FVector PosicionActual = GetActorLocation();
        if (!PosicionActual.Equals(PosicionInicialAnclaje, 5.f))
        {
            FVector RegresoSuave = FMath::VInterpTo(PosicionActual, PosicionInicialAnclaje, DeltaTime, 2.0f);
            SetActorLocation(RegresoSuave);
        }
    }
}

void ATribunalTormenta::ActivarJefe()
{
    bEstaActivo = true;
    bPuedeMoverse = true;
    PuntoDestinoActual = PosicionInicialAnclaje;

    CronometroRayo = 0.f;
    CronometroPlasma = 0.f;
}

void ATribunalTormenta::CalcularNuevoPuntoAleatorio()
{
    if (!bEstaActivo) return;

    float DesfaseX = FMath::FRandRange(-RadioMaximoMovimiento, RadioMaximoMovimiento);
    float DesfaseY = FMath::FRandRange(-RadioMaximoMovimiento, RadioMaximoMovimiento);

    PuntoDestinoActual = PosicionInicialAnclaje + FVector(DesfaseX, DesfaseY, 0.f);
    bPuedeMoverse = true;
}

void ATribunalTormenta::DispararRayo()
{
    APawn* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Jugador && ClaseRayo)
    {
        FVector MiPosicion = GetActorLocation();

        FVector Direccion = (Jugador->GetActorLocation() - MiPosicion);
        Direccion.Z = 0.f;
        Direccion.Normalize();

        FVector PuntoSpawnExterior = MiPosicion + (Direccion * 350.f);
        PuntoSpawnExterior.Z = MiPosicion.Z;

        FRotator RotacionPlana = Direccion.Rotation();
        RotacionPlana.Pitch = 0.f;
        RotacionPlana.Roll = 0.f;

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AProyectilRayo* Proyectil = GetWorld()->SpawnActor<AProyectilRayo>(ClaseRayo, PuntoSpawnExterior, RotacionPlana, SpawnParams);
        if (Proyectil)
        {
            Proyectil->DireccionVuelo = Direccion;
        }
    }
}

void ATribunalTormenta::SpawnPlasma()
{
    APawn* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Jugador && ClasePlasma)
    {
        FVector MiPosicion = GetActorLocation();

        FVector DireccionBase = (Jugador->GetActorLocation() - MiPosicion);
        DireccionBase.Z = 0.f;
        DireccionBase.Normalize();

        FRotator BaseRotation = DireccionBase.Rotation();
        BaseRotation.Pitch = 0.f;
        BaseRotation.Roll = 0.f;

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        float Angulos[3] = { -20.f, 0.f, 20.f };
        for (int32 i = 0; i < 3; i++)
        {
            FRotator RotacionDisparo = BaseRotation;
            RotacionDisparo.Yaw += Angulos[i];

            FVector DireccionProyectil = RotacionDisparo.Vector();
            DireccionProyectil.Z = 0.f;
            DireccionProyectil.Normalize();

            FVector PuntoSpawnExterior = MiPosicion + (DireccionProyectil * 350.f);
            PuntoSpawnExterior.Z = MiPosicion.Z;

            AProyectilPlasma* Proyectil = GetWorld()->SpawnActor<AProyectilPlasma>(ClasePlasma, PuntoSpawnExterior, RotacionDisparo, SpawnParams);
            if (Proyectil)
            {
                Proyectil->DireccionVuelo = DireccionProyectil;
            }
        }
    }
}
