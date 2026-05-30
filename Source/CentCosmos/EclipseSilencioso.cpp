// Fill out your copyright notice in the Description page of Project Settings.

#include "EclipseSilencioso.h"
#include "CentCosmosPawn.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AEclipseSilencioso::AEclipseSilencioso()
{
    PrimaryActorTick.bCanEverTick = true;

    EclipseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EclipseMesh"));
    RootComponent = EclipseMesh;

    // Carga la malla del cono del StarterContent
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone"));
    if (MeshAsset.Succeeded())
    {
        EclipseMesh->SetStaticMesh(MeshAsset.Object);
    }
}

void AEclipseSilencioso::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(20.0f);

    GetWorldTimerManager().SetTimer(DashTimerHandle, this, &AEclipseSilencioso::RealizarDash, TiempoEntreDash, true);
}

void AEclipseSilencioso::RealizarDash()
{
    APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (PlayerPawn)
    {
        FVector Dir = (PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        FVector NuevaPos = GetActorLocation() + (Dir * DistanciaPorDash);
        SetActorLocation(NuevaPos);
    }
}

void AEclipseSilencioso::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (PlayerPawn && FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation()) < 150.0f)
    {
        AplicarSilencio(PlayerPawn);
    }
}

void AEclipseSilencioso::AplicarSilencio(AActor* OtherActor)
{
    ACentCosmosPawn* Nave = Cast<ACentCosmosPawn>(OtherActor);
    if (Nave)
    {
        Nave->bPuedeDisparar = false; // Usamos tu variable original

        FTimerHandle SilencioHandle;
        GetWorldTimerManager().SetTimer(SilencioHandle, [Nave]()
            {
                if (Nave) Nave->bPuedeDisparar = true; // Restauramos
            }, 5.0f, false);

        Destroy();
    }
}