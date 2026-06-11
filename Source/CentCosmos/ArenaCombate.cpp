// Fill out your copyright notice in the Description page of Project Settings.

#include "ArenaCombate.h"
#include "TribunalBase.h" 
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AArenaCombate::AArenaCombate()
{
    PrimaryActorTick.bCanEverTick = false;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));

    RootEscena = CreateDefaultSubobject<USceneComponent>(TEXT("RootEscena"));
    RootComponent = RootEscena;

    Suelo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Suelo"));
    Suelo->SetupAttachment(RootComponent);
    if (CubeAsset.Succeeded()) Suelo->SetStaticMesh(CubeAsset.Object);
    Suelo->SetRelativeScale3D(FVector(30.f, 30.f, 0.1f));
    Suelo->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    auto ConfigurarPared = [&](UStaticMeshComponent* Pared, FVector Pos, FVector Escala) {
        Pared->SetupAttachment(RootComponent);
        Pared->SetStaticMesh(CubeAsset.Object);
        Pared->SetRelativeLocation(Pos);
        Pared->SetRelativeScale3D(Escala);
        Pared->SetCollisionProfileName(TEXT("BlockAll"));
        };

    ParedNorte = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParedNorte"));
    ConfigurarPared(ParedNorte, FVector(1500, 0, 240), FVector(0.5, 31.5, 5));

    ParedSur = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParedSur"));
    ConfigurarPared(ParedSur, FVector(-1500, 0, 240), FVector(0.5, 31.5, 5));

    ParedEste = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParedEste"));
    ConfigurarPared(ParedEste, FVector(0, 1500, 240), FVector(31.5, 0.5, 5));

    ParedOeste = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParedOeste"));
    ConfigurarPared(ParedOeste, FVector(0, -1500, 240), FVector(31.5, 0.5, 5));

    TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerArea"));
    TriggerArea->SetupAttachment(RootComponent);
    TriggerArea->SetBoxExtent(FVector(1450.f, 1450.f, 400.f));
    TriggerArea->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AArenaCombate::BeginPlay()
{
    Super::BeginPlay();
    TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &AArenaCombate::AlEntrarEnArena);
}

void AArenaCombate::AlEntrarEnArena(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APawn* PosibleJugador = Cast<APawn>(OtherActor);

    if (PosibleJugador && PosibleJugador->IsPlayerControlled())
    {
        UE_LOG(LogTemp, Warning, TEXT("Arena: Jugador detectado. Buscando Jefes..."));

        TArray<AActor*> Jefes;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATribunalBase::StaticClass(), Jefes);

        if (Jefes.Num() == 0)
        {
            UE_LOG(LogTemp, Error, TEXT("Arena: NO ENCONTRE NINGUN JEFE Revisa que el Jefe sea hijo de ATribunalBase"));
        }

        for (AActor* Actor : Jefes)
        {
            ATribunalBase* Jefe = Cast<ATribunalBase>(Actor);
            if (Jefe)
            {
                UE_LOG(LogTemp, Warning, TEXT("Arena: Activando Jefe!"));
                Jefe->ActivarJefe();
            }
        }
    }
}
