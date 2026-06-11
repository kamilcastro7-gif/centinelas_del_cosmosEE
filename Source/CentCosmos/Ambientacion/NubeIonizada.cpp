// Fill out your copyright notice in the Description page of Project Settings.

#include "NubeIonizada.h"
#include "Components/StaticMeshComponent.h"
#include "../Player/CentCosmosPawn.h"
#include "TimerManager.h"

ANubeIonizada::ANubeIonizada()
{
    PrimaryActorTick.bCanEverTick = true;

    MallaNube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaNube"));
    RootComponent = MallaNube;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneAsset(TEXT("StaticMesh'/Game/Assetssss/A3/Meshy_AI_Ember_Nebula_0607042041_texture.Meshy_AI_Ember_Nebula_0607042041_texture'"));
    if (PlaneAsset.Succeeded()) MallaNube->SetStaticMesh(PlaneAsset.Object);

    // --- CONFIGURACI�N DE COLISI�N ---
    MallaNube->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    MallaNube->SetCollisionObjectType(ECC_WorldDynamic);
    MallaNube->SetCollisionResponseToAllChannels(ECR_Ignore);
    MallaNube->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    MallaNube->SetGenerateOverlapEvents(true);

    MallaNube->SetRelativeScale3D(FVector(6.0f, 6.0f, 6.0f));

    // Valores por defecto
    VelocidadY = 200.0f;
    DuracionEfecto = 4.0f;
    RangoMovimientoY = 600.0f; // Define el �rea de patrulla
    DireccionMovimiento = 1.0f;
}

void ANubeIonizada::BeginPlay()
{
    Super::BeginPlay();
    PosicionInicialY = GetActorLocation().Y; // Guardamos el centro
}

void ANubeIonizada::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 1. Obtenemos la posici�n actual
    FVector CurrentLocation = GetActorLocation();

    // 2. Calculamos el desplazamiento basado en el tiempo
    // Usamos FMath::Sin para un movimiento fluido y autom�tico que nunca se sale del rango
    float Tiempo = GetWorld()->GetTimeSeconds();

    // El movimiento es: PosicionInicial + (Direccion * Velocidad * Tiempo)
    // Usamos Sin para que oscile perfectamente entre -Rango y +Rango
    float Desplazamiento = FMath::Sin(Tiempo * (VelocidadY / 100.0f)) * RangoMovimientoY;

    CurrentLocation.Y = PosicionInicialY + Desplazamiento;

    // 3. Aplicamos la nueva posici�n
    SetActorLocation(CurrentLocation);
}

void ANubeIonizada::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    ACentCosmosPawn* Nave = Cast<ACentCosmosPawn>(OtherActor);

    if (Nave)
    {
        // 1. Guardamos velocidad y ralentizamos (40% de reducci�n = 0.6)
        float VelocidadOriginal = Nave->MoveSpeed;
        Nave->MoveSpeed = VelocidadOriginal * 0.6f;

        // 2. Timer para restaurar
        FTimerHandle Handle;
        GetWorldTimerManager().SetTimer(Handle, [Nave, VelocidadOriginal]()
            {
                if (Nave) Nave->MoveSpeed = VelocidadOriginal;
            }, DuracionEfecto, false);

        // 3. �Desaparecer!
        Destroy();
    }
}