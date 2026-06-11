// Fill out your copyright notice in the Description page of Project Settings.

#include "TribunalSupremo.h"
#include "PRafaga.h"
#include "PSupremo.h" 
#include "RayoSupremo.h" 
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h" 
#include "Kismet/KismetMathLibrary.h" 

ATribunalSupremo::ATribunalSupremo()
{
    PrimaryActorTick.bCanEverTick = true;
    bEstaActivo = false;

    RangoDeteccionManual = 2500.f;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Assetssss/A16/Meshy_AI_The_Ember_Spire_0607221638_texture.Meshy_AI_The_Ember_Spire_0607221638_texture'"));
    if (MallaTribunal && MeshAsset.Succeeded())
    {
        MallaTribunal->SetStaticMesh(MeshAsset.Object);
        MallaTribunal->SetCollisionProfileName(TEXT("BlockAll"));
        MallaTribunal->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }

    SetActorScale3D(FVector(3.5f, 3.5f, 3.5f));
    Vida = 300.0f;
    VelocidadPatrulla = 600.f;
    AmplitudMovimiento = 600.f;
    DireccionMovimiento = 1.f;
    bEstaDisparando = false;
    PuntaActualEstrella = 0;
    RafagasDisparadasSupremo = 0;
    DireccionActualSupremo = 0;
}

void ATribunalSupremo::BeginPlay()
{
    Super::BeginPlay();
    PosicionInicial = GetActorLocation();
}

void ATribunalSupremo::Tick(float DeltaTime)
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

    if (!bEstaActivo)
    {
        FVector PosicionActual = GetActorLocation();
        if (!PosicionActual.Equals(PosicionInicial, 5.f))
        {
            FVector RegresoSuave = FMath::VInterpTo(PosicionActual, PosicionInicial, DeltaTime, 2.0f);
            SetActorLocation(RegresoSuave);
        }
        return;
    }

    if (!bEstaDisparando)
    {
        FVector UbicacionActual = GetActorLocation();
        UbicacionActual.Y += DireccionMovimiento * VelocidadPatrulla * DeltaTime;
        SetActorLocation(UbicacionActual);

        if (FMath::Abs(UbicacionActual.Y - PosicionInicial.Y) >= AmplitudMovimiento)
        {
            DireccionMovimiento *= -1.f;
        }
    }
}

void ATribunalSupremo::ActivarJefe()
{
    Super::ActivarJefe();
    bEstaActivo = true;

    GetWorld()->GetTimerManager().SetTimer(TimerCicloAtaque, this, &ATribunalSupremo::ControlarFases, 5.0f, true, 2.0f);

    UE_LOG(LogTemp, Warning, TEXT("TribunalSupremo: El jugador ha entrado a la arena. Iniciando aniquilacion!"));
}

void ATribunalSupremo::DesactivarJefe()
{
    bEstaActivo = false;
    bEstaDisparando = false;

    GetWorld()->GetTimerManager().ClearTimer(TimerCicloAtaque);
    GetWorld()->GetTimerManager().ClearTimer(TimerDisparoPunta);
    GetWorld()->GetTimerManager().ClearTimer(TimerDisparoSupremoContinua);
    GetWorld()->GetTimerManager().ClearTimer(TimerDescongelarRayo);

    UE_LOG(LogTemp, Warning, TEXT("TribunalSupremo: Jugador fuera de rango. Apagando sistemas."));
}

void ATribunalSupremo::ControlarFases()
{
    if (bEstaDisparando || !bEstaActivo) return;

    bEstaDisparando = true;

    int32 AtaqueAleatorio = FMath::RandRange(0, 2);

    if (AtaqueAleatorio == 0)
    {
        PuntaActualEstrella = 0;
        GetWorld()->GetTimerManager().SetTimer(TimerDisparoPunta, this, &ATribunalSupremo::DispararPuntaSecuencial, 0.35f, true, 0.1f);
    }
    else if (AtaqueAleatorio == 1)
    {
        RafagasDisparadasSupremo = 0;
        DireccionActualSupremo = 0;

        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (PlayerPawn)
        {
            RotacionHaciaJugadorBase = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
            RotacionHaciaJugadorBase.Pitch = 0.f;
            RotacionHaciaJugadorBase.Roll = 0.f;
        }
        else
        {
            RotacionHaciaJugadorBase = FRotator(0.f, -90.f, 0.f);
        }

        GetWorld()->GetTimerManager().SetTimer(TimerDisparoSupremoContinua, this, &ATribunalSupremo::DispararPSupremoContinuo, 0.08f, true, 0.1f);
    }
    else
    {
        UWorld* World = GetWorld();
        if (World)
        {
            APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
            FRotator RotacionHaciaJugador = FRotator(0.f, -90.f, 0.f);

            // Valor por defecto por si no hay jugador
            float AlturaZ = GetActorLocation().Z;

            if (PlayerPawn)
            {
                RotacionHaciaJugador = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
                RotacionHaciaJugador.Pitch = 0.f;
                RotacionHaciaJugador.Roll = 0.f;
                // Capturamos la altura del jugador
                AlturaZ = PlayerPawn->GetActorLocation().Z;
            }

            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            FVector UbicacionSpawn = GetActorLocation() + (RotacionHaciaJugador.Vector() * 100.f);

            // =========================================================================
            // CORRECCIÓN: EL RAYO NACE A LA ALTURA DEL JUGADOR
            // =========================================================================
            UbicacionSpawn.Z = AlturaZ;

            World->SpawnActor<ARayoSupremo>(ARayoSupremo::StaticClass(), UbicacionSpawn, RotacionHaciaJugador, SpawnParams);

            World->GetTimerManager().SetTimer(TimerDescongelarRayo, this, &ATribunalSupremo::TerminarAtaqueRayo, 4.0f, false);
        }
    }
}

void ATribunalSupremo::TerminarAtaqueRayo()
{
    bEstaDisparando = false;
}

void ATribunalSupremo::DispararPuntaSecuencial()
{
    UWorld* World = GetWorld();
    if (!World) return;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
    float AlturaZ = PlayerPawn ? PlayerPawn->GetActorLocation().Z : GetActorLocation().Z;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    float AnguloBasePunta = PuntaActualEstrella * 45.0f;

    for (int32 i = 0; i < 4; ++i)
    {
        float SeparacionLateral = (i - 1.5f) * 90.0f;
        FVector DireccionEstrella = FRotator(0.f, AnguloBasePunta, 0.f).Vector();
        FVector DerechaEstrella = FRotator(0.f, AnguloBasePunta + 90.f, 0.f).Vector();

        FVector UbicacionSpawn = GetActorLocation() + (DireccionEstrella * 120.f) + (DerechaEstrella * SeparacionLateral);

        // =========================================================================
        // CORRECCIÓN: LAS RÁFAGAS EN ESTRELLA NACEN A LA ALTURA DEL JUGADOR
        // =========================================================================
        UbicacionSpawn.Z = AlturaZ;

        World->SpawnActor<APRafaga>(APRafaga::StaticClass(), UbicacionSpawn, FRotator(0.f, AnguloBasePunta, 0.f), SpawnParams);
    }

    PuntaActualEstrella++;

    if (PuntaActualEstrella >= 8)
    {
        World->GetTimerManager().ClearTimer(TimerDisparoPunta);
        bEstaDisparando = false;
    }
}

void ATribunalSupremo::DispararPSupremoContinuo()
{
    UWorld* World = GetWorld();
    if (!World) return;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
    float AlturaZ = PlayerPawn ? PlayerPawn->GetActorLocation().Z : GetActorLocation().Z;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    float Desviaciones[4] = { -30.0f, -10.0f, 10.0f, 30.0f };
    float VariacionAngular = Desviaciones[DireccionActualSupremo];

    FRotator RotacionFinalProyectil = RotacionHaciaJugadorBase;
    RotacionFinalProyectil.Yaw += VariacionAngular;

    FVector DireccionDisparo = RotacionFinalProyectil.Vector();
    FVector UbicacionSpawn = GetActorLocation() + (DireccionDisparo * 130.f);

    // =========================================================================
    // CORRECCIÓN: LAS BALAS CONTINUAS NACEN A LA ALTURA DEL JUGADOR
    // =========================================================================
    UbicacionSpawn.Z = AlturaZ;

    World->SpawnActor<APSupremo>(APSupremo::StaticClass(), UbicacionSpawn, RotacionFinalProyectil, SpawnParams);

    DireccionActualSupremo = (DireccionActualSupremo + 1) % 4;
    RafagasDisparadasSupremo++;

    if (RafagasDisparadasSupremo >= 40)
    {
        World->GetTimerManager().ClearTimer(TimerDisparoSupremoContinua);
        bEstaDisparando = false;
    }
}
