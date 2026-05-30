// Fill out your copyright notice in the Description page of Project Settings.

#include "EVigia_Del_Vacio.h"
#include "ProyectilBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"     
#include "Kismet/KismetMathLibrary.h"  
#include "GameFramework/ProjectileMovementComponent.h"

AEVigia_Del_Vacio::AEVigia_Del_Vacio() {
    // 1. BALANCE: Velocidad reducida drásticamente para que sea más fácil apuntarle
    VelocidadVigia = 0.65f;
    AmplitudPatrulla = 500.0f;
    Tags.Add(FName("Enemigo"));

    VidaActual = 10.0f;
    DanioDeChoque = 10.0f;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
    if (MeshAsset.Succeeded()) {
        MallaEnemigo->SetStaticMesh(MeshAsset.Object);
    }

    // 2. BALANCE: Aumentamos su tamaño 2.5 veces para que tenga una "hitbox" más generosa
    MallaEnemigo->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));

    MallaEnemigo->SetSimulatePhysics(false);
    MallaEnemigo->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AEVigia_Del_Vacio::BeginPlay() {
    Super::BeginPlay();
    PosicionInicialSpawn = GetActorLocation();
    FTimerHandle TimerHandle_Ataque;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Ataque, this, &AEVigia_Del_Vacio::VigAtacar, 5.0f, true);
}

void AEVigia_Del_Vacio::moverVigia() {
    float Tiempo = GetWorld()->GetTimeSeconds() * VelocidadVigia;
    float Radio = 1200.0f;
    float OffsetAngulo = FMath::Sin(Tiempo) * 1.5f;

    FVector NuevaPos;
    NuevaPos.X = PosicionInicialSpawn.X + (FMath::Cos(OffsetAngulo) * Radio);
    NuevaPos.Y = PosicionInicialSpawn.Y + (FMath::Sin(OffsetAngulo) * Radio);
    NuevaPos.Z = PosicionInicialSpawn.Z;

    SetActorLocation(NuevaPos);

    AActor* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Jugador) {
        FRotator RotacionAlPlayer = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Jugador->GetActorLocation());
        SetActorRotation(RotacionAlPlayer);
    }
}

void AEVigia_Del_Vacio::VigAtacar() {
    if (!GetWorld()) return;

    FVector Ubicacion = GetActorLocation() + (GetActorForwardVector() * 120.f);
    AActor* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    FRotator Rotacion = (Jugador) ? UKismetMathLibrary::FindLookAtRotation(Ubicacion, Jugador->GetActorLocation()) : GetActorRotation();

    AProyectilBase* Proy = GetWorld()->SpawnActor<AProyectilBase>(AProyectilBase::StaticClass(), Ubicacion, Rotacion);
    if (Proy) {
        Proy->Danio = 10.0f;

        Proy->MallaProyectil->SetRelativeScale3D(FVector(0.8f));
        Proy->InitialLifeSpan = 5.0f;

        if (Proy->MovimientoProyectil) {
            Proy->MovimientoProyectil->InitialSpeed = 2000.f;
            Proy->MovimientoProyectil->MaxSpeed = 2000.f;
            Proy->MovimientoProyectil->Velocity = Rotacion.Vector() * 2000.f;
        }
    }
}