// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "CentCosmosPawn.h" 
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AProyectilBase::AProyectilBase()
{
    PrimaryActorTick.bCanEverTick = true;

    MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaProyectil"));
    RootComponent = MallaProyectil;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
    if (MeshAsset.Succeeded()) {
        MallaProyectil->SetStaticMesh(MeshAsset.Object);
    }

    MallaProyectil->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    MallaProyectil->SetCollisionResponseToAllChannels(ECR_Ignore); 
    MallaProyectil->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); 
    MallaProyectil->SetGenerateOverlapEvents(true);

    MallaProyectil->OnComponentBeginOverlap.AddDynamic(this, &AProyectilBase::OnOverlap);

    MallaProyectil->SetHiddenInGame(true);

    EfectoProyectil = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoProyectil"));
    EfectoProyectil->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> EnergiaAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Energy_1.NS_Energy_1'"));
    if (EnergiaAsset.Succeeded()) {
        EfectoProyectil->SetAsset(EnergiaAsset.Object);
    }

    MovimientoProyectil = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovimientoProyectil"));
    MovimientoProyectil->bRotationFollowsVelocity = true;
    MovimientoProyectil->ProjectileGravityScale = 0.0f;

    VidaProyectil = 1;
    bEsSeguidor = false;
    InitialLifeSpan = 5.0f;
    Danio = 1.0f;
}

void AProyectilBase::BeginPlay() { Super::BeginPlay(); }

void AProyectilBase::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (bEsSeguidor) {
        AActor* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (Jugador) {

            FRotator RotacionObjetivo = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Jugador->GetActorLocation());
            FRotator NuevaRotacion = FMath::RInterpTo(GetActorRotation(), RotacionObjetivo, DeltaTime, 2.0f);

            SetActorRotation(NuevaRotacion);

            MovimientoProyectil->Velocity = NuevaRotacion.Vector() * MovimientoProyectil->MaxSpeed;
        }
    }
}

void AProyectilBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherActor != GetOwner())
    {
        ACentCosmosPawn* NaveJugador = Cast<ACentCosmosPawn>(OtherActor);
        if (NaveJugador)
        {
            NaveJugador->RecibirDanioNave(Danio);
            Destroy();
        }
    }
}

