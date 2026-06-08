// Fill out your copyright notice in the Description page of Project Settings.

#include "RayoSupremo.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "../Player/CentCosmosPawn.h"
#include "NiagaraComponent.h"

ARayoSupremo::ARayoSupremo()
{
    PrimaryActorTick.bCanEverTick = true;

    RaizGrupo = CreateDefaultSubobject<USceneComponent>(TEXT("RaizGrupo"));
    RootComponent = RaizGrupo;

    // --- 1. MALLA DE ADVERTENCIA ---
    MallaAdvertenciaComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaAdvertenciaComp"));
    MallaAdvertenciaComp->SetupAttachment(RaizGrupo);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAdvAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    if (MeshAdvAsset.Succeeded()) MallaAdvertenciaComp->SetStaticMesh(MeshAdvAsset.Object);

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatAdvAsset(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse'"));
    if (MatAdvAsset.Succeeded()) MallaAdvertenciaComp->SetMaterial(0, MatAdvAsset.Object);

    MallaAdvertenciaComp->SetRelativeScale3D(FVector(60.0f, 0.05f, 0.2f));
    MallaAdvertenciaComp->SetRelativeLocation(FVector(3000.f, 0.f, 0.f));
    MallaAdvertenciaComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


    // --- 2. MALLA LETAL (Ahor es puramente de relleno visual, SIN físicas) ---
    MallaLetalComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaLetalComp"));
    MallaLetalComp->SetupAttachment(RaizGrupo);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshLetalAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
    if (MeshLetalAsset.Succeeded()) MallaLetalComp->SetStaticMesh(MeshLetalAsset.Object);

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatLetalAsset(TEXT("Material'/Game/StarterContent/Materials/M_Lava_With_Blends.M_Lava_With_Blends'"));
    if (MatLetalAsset.Succeeded()) MallaLetalComp->SetMaterial(0, MatLetalAsset.Object);

    MallaLetalComp->SetRelativeScale3D(FVector(60.0f, 4.0f, 4.0f));
    MallaLetalComp->SetRelativeLocation(FVector(3000.f, 0.f, 0.f));
    MallaLetalComp->SetVisibility(false);
    MallaLetalComp->SetHiddenInGame(true);

    // Apagamos la colisión de la malla para evitar bugs. ¡Usaremos matemáticas en el Tick!
    MallaLetalComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


    // --- 3. EFECTO NIAGARA ---
    EfectoRayoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoRayoNiagara"));
    EfectoRayoNiagara->SetupAttachment(RaizGrupo);

    EfectoRayoNiagara->SetRelativeLocation(FVector(150.f, 0.f, 0.f));
    EfectoRayoNiagara->SetRelativeScale3D(FVector(8.0f, 1.5f, 1.5f));

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> RayoAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Ray_5.NS_Ray_5'"));
    if (RayoAsset.Succeeded())
    {
        EfectoRayoNiagara->SetAsset(RayoAsset.Object);
    }

    EfectoRayoNiagara->SetAutoActivate(false);
    bEsRayoLetal = false;
}

void ARayoSupremo::BeginPlay()
{
    Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(TimerFaseLetal, this, &ARayoSupremo::ActivarRayoLetal, 2.0f, false);
    SetLifeSpan(4.0f);
}

void ARayoSupremo::ActivarRayoLetal()
{
    bEsRayoLetal = true;
    if (MallaAdvertenciaComp) MallaAdvertenciaComp->SetVisibility(false);
    if (EfectoRayoNiagara) EfectoRayoNiagara->Activate();
}

// =========================================================================
// ESCANEO MATEMÁTICO INFALIBLE: MATA TODO LO QUE TOQUE LA CAJA
// =========================================================================
// =========================================================================
// ESCANEO MATEMÁTICO ABSOLUTO: IGNORA COLISIONES Y USA GEOMETRÍA PURA
// =========================================================================
void ARayoSupremo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bEsRayoLetal)
    {
        // 1. Obtenemos directamente a tu jugador (sin usar físicas)
        APawn* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (Jugador)
        {
            FVector OrigenRayo = GetActorLocation();
            FVector DireccionRayo = GetActorForwardVector();
            FVector PosicionJugador = Jugador->GetActorLocation();

            // 2. Calculamos un vector desde el inicio del rayo hacia la nave
            FVector HaciaJugador = PosicionJugador - OrigenRayo;

            // 3. Calculamos qué tan "adelante" está la nave respecto al rayo
            float Proyeccion = FVector::DotProduct(HaciaJugador, DireccionRayo);

            // 4. Verificamos si la nave está frente al cañón (Proyeccion > 0) 
            // y dentro del alcance del láser (ej. 6000 unidades)
            if (Proyeccion > 0.f && Proyeccion < 6000.f)
            {
                // 5. Calculamos el punto exacto del láser que pasa al lado de la nave
                FVector PuntoEnElRayo = OrigenRayo + (DireccionRayo * Proyeccion);

                // 6. Medimos la distancia desde ese punto del láser hasta la nave
                float DistanciaAlLaser = FVector::Dist(PosicionJugador, PuntoEnElRayo);

                // 7. Si la nave está a menos de 350 unidades del centro del rayo (su grosor) ¡BAM!
                if (DistanciaAlLaser < 350.f)
                {
                    ACentCosmosPawn* Nave = Cast<ACentCosmosPawn>(Jugador);
                    if (Nave)
                    {
                        Nave->RecibirDanioNave(9999.0f);
                    }
                }
            }
        }
    }
}

