// Fill out your copyright notice in the Description page of Project Settings.

#include "EVigia_Del_Vacio.h"
#include "ProyectilBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h" // NUEVO
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"     
#include "Kismet/KismetMathLibrary.h"  
#include "GameFramework/ProjectileMovementComponent.h"

AEVigia_Del_Vacio::AEVigia_Del_Vacio() {
    // 1. BALANCE: Velocidad reducida drásticamente
    VelocidadVigia = 0.65f;
    AmplitudPatrulla = 500.0f;
    Tags.Add(FName("Enemigo"));

    VidaActual = 11.0f;
    DanioDeChoque = 10.0f;

    // 2. EL TRUCO DE LA HITBOX: La cápsula se vuelve la colisión invisible
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
    if (MeshAsset.Succeeded()) {
        MallaEnemigo->SetStaticMesh(MeshAsset.Object);
    }

    MallaEnemigo->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
    MallaEnemigo->SetSimulatePhysics(false);

    // Lo volvemos invisible en el juego, pero mantenemos su solidez
    MallaEnemigo->SetHiddenInGame(true);
    MallaEnemigo->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MallaEnemigo->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MallaEnemigo->SetGenerateOverlapEvents(true);

    // 3. MALLA DEL DRON: Solo como componente visual
    MallaDrone = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MallaDrone"));
    if (RootComponent) {
        MallaDrone->SetupAttachment(RootComponent); // Se pega a la cápsula invisible
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> DroneAsset(TEXT("SkeletalMesh'/Game/Scifi_Survey_Drones/Drone_01/Meshes/SK_Sci-fi_Survey_Drone_01.SK_Sci-fi_Survey_Drone_01'"));
    if (DroneAsset.Succeeded()) {
        MallaDrone->SetSkeletalMesh(DroneAsset.Object);
    }
    MallaDrone->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
    // Apagamos la colisión del dron visual para que no interfiera con los impactos a la cápsula
    MallaDrone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEVigia_Del_Vacio::BeginPlay() {
    Super::BeginPlay();
    PosicionInicialSpawn = GetActorLocation();
    FTimerHandle TimerHandle_Ataque;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Ataque, this, &AEVigia_Del_Vacio::VigAtacar, 5.0f, true);
}

void AEVigia_Del_Vacio::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // Llamamos a la función para que el Vigía patrulle constantemente
    moverVigia();
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

    // Aumentamos a 150.f la separación para que el proyectil no choque con la cápsula invisible
    FVector Ubicacion = GetActorLocation() + (GetActorForwardVector() * 150.f);
    AActor* Jugador = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    FRotator Rotacion = (Jugador) ? UKismetMathLibrary::FindLookAtRotation(Ubicacion, Jugador->GetActorLocation()) : GetActorRotation();

    // Añadimos SpawnParams para evitar los problemas que tuvimos con el vástago
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Instigator = GetInstigator();

    AProyectilBase* Proy = GetWorld()->SpawnActor<AProyectilBase>(AProyectilBase::StaticClass(), Ubicacion, Rotacion, SpawnParams);

    if (Proy) {
        Proy->Danio = 10.0f;

        if (Proy->MallaProyectil) {
            Proy->MallaProyectil->SetRelativeScale3D(FVector(0.8f));
        }

        Proy->InitialLifeSpan = 5.0f;

        if (Proy->MovimientoProyectil) {
            Proy->MovimientoProyectil->InitialSpeed = 2000.f;
            Proy->MovimientoProyectil->MaxSpeed = 2000.f;
            Proy->MovimientoProyectil->Velocity = Rotacion.Vector() * 2000.f;
        }
    }
}