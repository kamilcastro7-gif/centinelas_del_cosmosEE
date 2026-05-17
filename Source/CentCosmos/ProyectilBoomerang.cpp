// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilBoomerang.h"
#include "Components/StaticMeshComponent.h"
#include "CentCosmosPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "ProyectilBase.h" 

AProyectilBoomerang::AProyectilBoomerang()
{
    PrimaryActorTick.bCanEverTick = true;

    // REFERENCIA: Shape_Torus
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TorusMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus'"));
    ProyectilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoomerangMesh"));
    RootComponent = ProyectilMesh;

    if (TorusMesh.Succeeded()) ProyectilMesh->SetStaticMesh(TorusMesh.Object);

    ProyectilMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ProyectilMesh->OnComponentBeginOverlap.AddDynamic(this, &AProyectilBoomerang::OnOverlap);

    Velocidad = 1500.f;
    TiempoDeGiro = 1.2f;
    TiempoEnVuelo = 0.f;
    bRegresando = false;
}

void AProyectilBoomerang::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TiempoEnVuelo += DeltaTime;

    if (!bRegresando && TiempoEnVuelo >= TiempoDeGiro) bRegresando = true;

    if (!bRegresando)
    {
        FVector NuevaPosicion = GetActorLocation() + (GetActorForwardVector() * Velocidad * DeltaTime);
        SetActorLocation(NuevaPosicion);
    }
    else if (NaveDueno)
    {
        FVector DireccionALaNave = (NaveDueno->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        FVector NuevaPosicion = GetActorLocation() + (DireccionALaNave * Velocidad * DeltaTime);

        AddActorLocalRotation(FRotator(0.f, 5.f, 0.f));
        SetActorLocation(NuevaPosicion);
    }
}

void AProyectilBoomerang::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        AProyectilBase* ProyectilEnemigo = Cast<AProyectilBase>(OtherActor);
        if (ProyectilEnemigo) ProyectilEnemigo->RecibirImpacto();

        if (bRegresando && OtherActor == NaveDueno)
        {
            if (NaveDueno) NaveDueno->bBoomerangEnVuelo = false;
            Destroy();
        }
    }
}

