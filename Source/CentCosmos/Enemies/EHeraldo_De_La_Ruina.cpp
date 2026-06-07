// Fill out your copyright notice in the Description page of Project Settings.

#include "EHeraldo_De_La_Ruina.h"
#include "Kismet/GameplayStatics.h"
#include "ProyectilBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h" // NUEVO
#include "GameFramework/ProjectileMovementComponent.h"

AEHeraldo_De_La_Ruina::AEHeraldo_De_La_Ruina() {
    // Es importante activar el Tick para este enemigo
    PrimaryActorTick.bCanEverTick = true;

    VelocidadHeraldo = 150.0f;
    bEstaPersiguiendo = true;
    TimerEstadoHeraldo = 0.0f;
    Tags.Add(FName("Enemigo"));

    VidaActual = 40.0f;
    DanioDeChoque = 5.0f;

    // 1. EL TRUCO DE LA HITBOX: El cubo base se vuelve invisible
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
    if (MeshAsset.Succeeded()) {
        MallaEnemigo->SetStaticMesh(MeshAsset.Object);
    }

    if (MallaEnemigo) {
        MallaEnemigo->SetHiddenInGame(true);
        MallaEnemigo->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        MallaEnemigo->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
        MallaEnemigo->SetGenerateOverlapEvents(true);
    }

    // 2. MALLA DEL HERALDO: Solo visual
    MallaHeraldo = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MallaHeraldo"));
    if (RootComponent) {
        MallaHeraldo->SetupAttachment(RootComponent); // Se pega al cubo invisible
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> DroneAsset(TEXT("SkeletalMesh'/Game/Scifi_Survey_Drones/Drone_02/Meshes/SK_Sci-fi_Survey_Drone_02.SK_Sci-fi_Survey_Drone_02'"));
    if (DroneAsset.Succeeded()) {
        MallaHeraldo->SetSkeletalMesh(DroneAsset.Object);
    }

    // --- ESCALA DEL ASSET --- 
    // Lo hacemos x4 veces más grande (Puedes subir a 5.0f o bajar a 3.0f según prefieras)
    MallaHeraldo->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));

    // Apagamos la colisión visual para que el cubo haga todo el trabajo matemático
    MallaHeraldo->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEHeraldo_De_La_Ruina::BeginPlay() {
    Super::BeginPlay();
    FTimerHandle TimerHandle_Ataque;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Ataque, this, &AEHeraldo_De_La_Ruina::HerAtacar, 10.0f, true);
}

// NUEVO: Ejecutamos el movimiento en cada frame
void AEHeraldo_De_La_Ruina::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    moverHeraldo();
}

void AEHeraldo_De_La_Ruina::moverHeraldo()
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    AActor* Jugador = UGameplayStatics::GetPlayerPawn(Mundo, 0);
    if (!Jugador) return;

    float DeltaTime = Mundo->GetDeltaSeconds();
    TimerEstadoHeraldo += DeltaTime;

    float TiempoLimite = bEstaPersiguiendo ? 15.0f : 3.0f;

    if (TimerEstadoHeraldo >= TiempoLimite)
    {
        TimerEstadoHeraldo = 0.0f;
        bEstaPersiguiendo = !bEstaPersiguiendo;
    }

    if (!bEstaPersiguiendo) return;

    FVector MiPosicion = GetActorLocation();
    FVector PosicionJugador = Jugador->GetActorLocation();

    FVector Direccion = (PosicionJugador - MiPosicion);
    Direccion.Z = 0.0f;

    if (Direccion.Size() > 10.0f)
    {
        FVector DireccionNormal = Direccion.GetSafeNormal();
        FVector Desplazamiento = DireccionNormal * VelocidadHeraldo * DeltaTime;

        FHitResult Hit;
        AddActorWorldOffset(Desplazamiento, true, &Hit);

        if (Hit.bBlockingHit)
        {
            AActor* Obstaculo = Hit.GetActor();

            if (Obstaculo && Obstaculo->ActorHasTag(FName("Enemigo")))
            {
                AddActorWorldOffset(Desplazamiento, false);
            }
            else
            {
                FVector Deslizamiento = FVector::VectorPlaneProject(Desplazamiento, Hit.Normal);
                AddActorWorldOffset(Deslizamiento, true);
            }
        }

        SetActorRotation(DireccionNormal.Rotation());
    }
}

void AEHeraldo_De_La_Ruina::HerAtacar() {
    if (!GetWorld()) return;

    AActor* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Jugador) return;

    FVector Ubicacion = GetActorLocation() + (GetActorForwardVector() * 200.f);
    FRotator Rotacion = GetActorRotation();

    // Parámetros de aparición para no lastimarse a sí mismo
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Instigator = GetInstigator();

    AProyectilBase* GranEsfera = GetWorld()->SpawnActor<AProyectilBase>(AProyectilBase::StaticClass(), Ubicacion, Rotacion, SpawnParams);

    if (GranEsfera) {
        GranEsfera->bEsSeguidor = true;
        GranEsfera->Danio = 5.0f;

        // Desaparece automáticamente tras 6 segundos de persecución
        GranEsfera->SetLifeSpan(6.0f);

        if (GranEsfera->MovimientoProyectil) {
            GranEsfera->MovimientoProyectil->InitialSpeed = 950.f;
            GranEsfera->MovimientoProyectil->MaxSpeed = 1200.f;
            GranEsfera->MovimientoProyectil->bShouldBounce = false;

            // Sistema Homing de Persecución Persistente
            GranEsfera->MovimientoProyectil->bIsHomingProjectile = true;
            GranEsfera->MovimientoProyectil->HomingTargetComponent = Jugador->GetRootComponent();

            // Fuerza de aceleración del giro
            GranEsfera->MovimientoProyectil->HomingAccelerationMagnitude = 4500.f;
        }
    }
}