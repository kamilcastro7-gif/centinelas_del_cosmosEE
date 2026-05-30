// Fill out your copyright notice in the Description page of Project Settings.

#include "TribAsedio.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ATribAsedio::ATribAsedio()
{
    PrimaryActorTick.bCanEverTick = true;

    MallaJefe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaJefe"));
    RootComponent = MallaJefe;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> TorusAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus'"));
    if (TorusAsset.Succeeded())
    {
        MallaJefe->SetStaticMesh(TorusAsset.Object);
    }

    MallaJefe->SetRelativeScale3D(FVector(5.f, 5.f, 2.f));

    TotalPlacas = 5;
    RadioOrbita = 650.f;
    VelocidadRotacion = 90.f;

    RangoDeteccion = 2200.f;
    bEstaActivo = false;
    bEsVulnerable = false;
    AnguloActualPlacas = 0.f;

    VelocidadDesplazamiento = 250.f;
    RadioMaximoMovimiento = 500.f;
    TiempoEsperaEnPunto = 1.5f;
    bPuedeMoverse = true;
}

void ATribAsedio::BeginPlay()
{
    Super::BeginPlay();
    PosicionInicialAnclaje = GetActorLocation();
    PuntoDestinoActual = PosicionInicialAnclaje;
}

void ATribAsedio::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    APawn* NaveJugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (NaveJugador)
    {
        float DistanciaAlJugador = FVector::Dist2D(GetActorLocation(), NaveJugador->GetActorLocation());

        if (DistanciaAlJugador <= RangoDeteccion)
        {
            if (!bEstaActivo)
            {
                bEstaActivo = true;
                SpawnEscudoPlacas();
                CalcularNuevoPuntoAleatorio();
            }
        }
        else
        {
            if (bEstaActivo)
            {
                bEstaActivo = false;
                DesactivarYLimpiarEscudo();
                PuntoDestinoActual = PosicionInicialAnclaje;
                bPuedeMoverse = true;
            }
        }
    }

    if (bEstaActivo)
    {
        FVector PosicionActual = GetActorLocation();

        if (bPuedeMoverse && !PosicionActual.Equals(PuntoDestinoActual, 10.f))
        {
            FVector NuevaPosicionJefe = FMath::VInterpTo(PosicionActual, PuntoDestinoActual, DeltaTime, VelocidadDesplazamiento / 100.f);
            NuevaPosicionJefe.Z = PosicionInicialAnclaje.Z;
            SetActorLocation(NuevaPosicionJefe);
        }
        else if (bPuedeMoverse)
        {
            bPuedeMoverse = false;
            GetWorld()->GetTimerManager().SetTimer(TimerEsperaMovimiento, this, &ATribAsedio::CalcularNuevoPuntoAleatorio, TiempoEsperaEnPunto, false);
        }

        AnguloActualPlacas += VelocidadRotacion * DeltaTime;
        if (AnguloActualPlacas >= 360.f) AnguloActualPlacas -= 360.f;

        FVector CentroJefe = GetActorLocation();
        float SeparacionAngular = 360.f / TotalPlacas;

        ArregloPlacas.Remove(nullptr);

        for (int32 i = 0; i < ArregloPlacas.Num(); ++i)
        {
            if (ArregloPlacas[i])
            {
                float AnguloRad = FMath::DegreesToRadians(AnguloActualPlacas + (i * SeparacionAngular));

                FVector NuevaPosicionPlaca = CentroJefe + FVector(FMath::Cos(AnguloRad) * RadioOrbita, FMath::Sin(AnguloRad) * RadioOrbita, 0.f);
                NuevaPosicionPlaca.Z = CentroJefe.Z;

                ArregloPlacas[i]->SetActorLocation(NuevaPosicionPlaca);

                FRotator NuevaRotacionPlaca = FRotator(0.f, FMath::RadiansToDegrees(AnguloRad) + 90.f, 0.f);
                ArregloPlacas[i]->SetActorRotation(NuevaRotacionPlaca);
            }
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

void ATribAsedio::CalcularNuevoPuntoAleatorio()
{
    if (!bEstaActivo) return;

    float DesfaseX = FMath::FRandRange(-RadioMaximoMovimiento, RadioMaximoMovimiento);
    float DesfaseY = FMath::FRandRange(-RadioMaximoMovimiento, RadioMaximoMovimiento);

    PuntoDestinoActual = PosicionInicialAnclaje + FVector(DesfaseX, DesfaseY, 0.f);
    bPuedeMoverse = true;
}

void ATribAsedio::SpawnEscudoPlacas()
{
    bEsVulnerable = false;
    FVector CentroJefe = GetActorLocation();
    float SeparacionAngular = 360.f / TotalPlacas;

    for (int32 i = 0; i < TotalPlacas; ++i)
    {
        float AnguloRad = FMath::DegreesToRadians(i * SeparacionAngular);
        FVector SpawnPos = CentroJefe + FVector(FMath::Cos(AnguloRad) * RadioOrbita, FMath::Sin(AnguloRad) * RadioOrbita, 0.f);
        SpawnPos.Z = CentroJefe.Z;

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        APlacaMetal* NuevaPlaca = GetWorld()->SpawnActor<APlacaMetal>(APlacaMetal::StaticClass(), SpawnPos, FRotator::ZeroRotator, SpawnParams);
        if (NuevaPlaca)
        {
            NuevaPlaca->OwnerJefe = this;
            ArregloPlacas.Add(NuevaPlaca);
        }
    }
}

void ATribAsedio::EliminarPlacaDeArreglo(APlacaMetal* PlacaMuerta)
{
    if (ArregloPlacas.Contains(PlacaMuerta))
    {
        ArregloPlacas.Remove(PlacaMuerta);

        if (ArregloPlacas.Num() == 0)
        {
            bEsVulnerable = true;
            GetWorld()->GetTimerManager().SetTimer(TimerVulnerabilidad, this, &ATribAsedio::RegenerarEscudo, 7.f, false);
        }
    }
}

void ATribAsedio::RegenerarEscudo()
{
    if (bEstaActivo)
    {
        SpawnEscudoPlacas();
    }
}

void ATribAsedio::DesactivarYLimpiarEscudo()
{
    GetWorld()->GetTimerManager().ClearTimer(TimerVulnerabilidad);
    GetWorld()->GetTimerManager().ClearTimer(TimerEsperaMovimiento);
    bEsVulnerable = false;

    for (APlacaMetal* Placa : ArregloPlacas)
    {
        if (Placa)
        {
            Placa->Destroy();
        }
    }
    ArregloPlacas.Empty();
}
