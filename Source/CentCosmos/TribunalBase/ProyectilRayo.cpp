// Fill out your copyright notice in the Description page of Project Settings.


#include "ProyectilRayo.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

AProyectilRayo::AProyectilRayo()
{
    PrimaryActorTick.bCanEverTick = true;

    MallaRayo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaRayo"));
    RootComponent = MallaRayo;

    MallaRayo->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaRayo->SetMobility(EComponentMobility::Movable);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Pipe.Shape_Pipe'"));
    if (MeshAsset.Succeeded()) MallaRayo->SetStaticMesh(MeshAsset.Object);

    VelocidadProyectil = 1500.f;
    DireccionVuelo = FVector::ZeroVector;

    MallaRayo->OnComponentBeginOverlap.AddDynamic(this, &AProyectilRayo::OnComponenteOverlap);
}

void AProyectilRayo::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(4.0f);
}

void AProyectilRayo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!DireccionVuelo.IsZero())
    {
        FVector NuevaPosicion = GetActorLocation() + (DireccionVuelo * VelocidadProyectil * DeltaTime);
        SetActorLocation(NuevaPosicion, false);
    }
}

void AProyectilRayo::OnComponenteOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor == GetOwner())
    {
        return;
    }

    APawn* Jugador = Cast<APawn>(OtherActor);
    if (Jugador)
    {
        APlayerController* PC = Cast<APlayerController>(Jugador->GetController());
        if (PC)
        {
            Jugador->DisableInput(PC);

            FTimerDelegate TimerDel;
            TWeakObjectPtr<APawn> WeakJugador = Jugador;
            TWeakObjectPtr<APlayerController> WeakPC = PC;

            TimerDel.BindLambda([WeakJugador, WeakPC]() {
                if (WeakJugador.IsValid() && WeakPC.IsValid())
                {
                    WeakJugador->EnableInput(WeakPC.Get());
                }
                });

            FTimerHandle RestoreTimerHandle;
            GetWorld()->GetTimerManager().SetTimer(RestoreTimerHandle, TimerDel, 1.0f, false);
        }

        Destroy();
    }
}

