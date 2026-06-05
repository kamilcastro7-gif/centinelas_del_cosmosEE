// Fill out your copyright notice in the Description page of Project Settings.

#include "EVastago_Del_infierno.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "ProyectilBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

AEVastago_Del_infierno::AEVastago_Del_infierno() {
    PrimaryActorTick.bCanEverTick = true;

    VelocidadVastago = 750.0f;
    DireccionMovimiento = FVector(1.0f, 1.0f, 0.0f);
    Tags.Add(FName("Enemigo"));

    TimerEstado = 0.0f;
    bEstaRodeando = false;
    bOlaCargaIniciada = false;

    // ESTADÍSTICAS VÁSTAGO
    VidaActual = 2.0f;
    DanioDeChoque = 1.0f;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
    if (MeshAsset.Succeeded()) MallaEnemigo->SetStaticMesh(MeshAsset.Object);

    ProyectilFalso = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProyectilFalso"));
    ProyectilFalso->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
    if (SphereAsset.Succeeded()) ProyectilFalso->SetStaticMesh(SphereAsset.Object);

    ProyectilFalso->SetRelativeLocation(FVector(60.f, 0.f, 0.f));
    ProyectilFalso->SetRelativeScale3D(FVector(0.01f));
    ProyectilFalso->SetVisibility(false);
    ProyectilFalso->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEVastago_Del_infierno::BeginPlay() {
    Super::BeginPlay();
}

void AEVastago_Del_infierno::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimerEstado += DeltaTime;
    float TiempoLimite = bEstaRodeando ? 7.0f : 5.0f;

    if (TimerEstado >= TiempoLimite) {
        if (bEstaRodeando) {
            EjecutarDisparo();
        }

        TimerEstado = 0.0f;
        bEstaRodeando = !bEstaRodeando;
        bOlaCargaIniciada = false;
        DireccionDispersion = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal();
    }

    if (bEstaRodeando && TimerEstado >= 1.5f && !bOlaCargaIniciada) {
        IntentarLiderarCarga();
    }

    moverVastago();
}

void AEVastago_Del_infierno::moverVastago() {
    AActor* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Jugador) return;

    float DeltaTime = GetWorld()->GetDeltaSeconds();

    FVector Destino;
    if (bEstaRodeando) {
        float Angulo = GetUniqueID() + (GetWorld()->GetTimeSeconds() * 0.8f);
        FVector Offset(FMath::Cos(Angulo) * 500.0f, FMath::Sin(Angulo) * 500.0f, 0.0f);
        Destino = Jugador->GetActorLocation() + Offset;
    }
    else {
        Destino = GetActorLocation() + (DireccionDispersion * 400.0f);
    }

    FVector NuevaPos = FMath::VInterpTo(GetActorLocation(), Destino, DeltaTime, 1.5f);
    SetActorLocation(NuevaPos);

    FRotator RotacionHaciaJugador = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Jugador->GetActorLocation());
    SetActorRotation(RotacionHaciaJugador);
}

void AEVastago_Del_infierno::IntentarLiderarCarga() {
    bOlaCargaIniciada = true;

    TArray<AActor*> Vastagos;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEVastago_Del_infierno::StaticClass(), Vastagos);

    APawn* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Jugador || Vastagos.Num() == 0) return;

    if (Vastagos[0] == this)
    {
        Vastagos.Sort([Jugador](const AActor& A, const AActor& B) {
            FVector DirA = A.GetActorLocation() - Jugador->GetActorLocation();
            FVector DirB = B.GetActorLocation() - Jugador->GetActorLocation();
            return FMath::Atan2(DirA.Y, DirA.X) < FMath::Atan2(DirB.Y, DirB.X);
            });

        float RitmoDeCarga = 0.35f;

        for (int32 i = 0; i < Vastagos.Num(); ++i)
        {
            AEVastago_Del_infierno* Vastago = Cast<AEVastago_Del_infierno>(Vastagos[i]);
            if (Vastago)
            {
                Vastago->bOlaCargaIniciada = true;
                FTimerHandle TimerCarga;
                float TiempoInicioCarga = FMath::Max(0.01f, i * RitmoDeCarga);
                GetWorld()->GetTimerManager().SetTimer(TimerCarga, Vastago, &AEVastago_Del_infierno::CargarAtaque, TiempoInicioCarga, false);
            }
        }
    }
}

void AEVastago_Del_infierno::CargarAtaque() {
    if (ProyectilFalso) {
        ProyectilFalso->SetVisibility(true);
        ProyectilFalso->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
    }
}

void AEVastago_Del_infierno::EjecutarDisparo() {
    if (ProyectilFalso) {
        ProyectilFalso->SetVisibility(false);
    }

    AActor* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Jugador) return;

    FVector DirHaciaJugador = (Jugador->GetActorLocation() - GetActorLocation());
    DirHaciaJugador.Z = 0.f;
    DirHaciaJugador.Normalize();

    FVector Ubicacion = GetActorLocation() + (DirHaciaJugador * 80.f);
    FRotator RotacionHaciaJugador = DirHaciaJugador.Rotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AProyectilBase* Proy = GetWorld()->SpawnActor<AProyectilBase>(AProyectilBase::StaticClass(), Ubicacion, RotacionHaciaJugador, SpawnParams);

    if (Proy) {
        Proy->Danio = 2.0f; // Daño del Vástago

        if (Proy->MallaProyectil) {
            Proy->MallaProyectil->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
        }

        Proy->InitialLifeSpan = 2.5f;

        if (Proy->MovimientoProyectil) {
            Proy->MovimientoProyectil->InitialSpeed = 600.f;
            Proy->MovimientoProyectil->MaxSpeed = 600.f;
            Proy->MovimientoProyectil->Velocity = RotacionHaciaJugador.Vector() * 600.f;
        }
    }
}