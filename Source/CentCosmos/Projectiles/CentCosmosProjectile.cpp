// Copyright Epic Games, Inc. All Rights Reserved.
#include "CentCosmosProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Enjambre.h"
#include "Escudo.h"
#include "../EExclusivo.h"
#include "TribunalBase/TribunalBase.h"
#include "GrietaAntimateria.h"
#include "TribunalBase/PlacaMetal.h"

ACentCosmosProjectile::ACentCosmosProjectile()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(
        TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
    ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
    ProjectileMesh->SetupAttachment(RootComponent);
    ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
    ProjectileMesh->OnComponentHit.AddDynamic(this, &ACentCosmosProjectile::OnHit);
    RootComponent = ProjectileMesh;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
    ProjectileMovement->UpdatedComponent = ProjectileMesh;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 6000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.f;

    InitialLifeSpan = 3.0f;
    Danio = 2.0f;
}

void ACentCosmosProjectile::BeginPlay()
{
    Super::BeginPlay();

    // Ignorar al dueño
    if (GetOwner())
        ProjectileMesh->IgnoreActorWhenMoving(GetOwner(), true);

    // Ignorar otros proyectiles del mismo dueño para evitar colisiones entre disparos triples
    TArray<AActor*> Proyectiles;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACentCosmosProjectile::StaticClass(), Proyectiles);
    for (AActor* P : Proyectiles)
        ProjectileMesh->IgnoreActorWhenMoving(P, true);
}

void ACentCosmosProjectile::ForzarDireccion(FVector Direccion, float Velocidad)
{
    if (!ProjectileMovement) return;
    FVector Dir = Direccion.GetSafeNormal();
    ProjectileMovement->StopMovementImmediately();
    ProjectileMovement->SetActive(false);
    ProjectileMovement->InitialSpeed = Velocidad;
    ProjectileMovement->MaxSpeed = Velocidad;
    ProjectileMovement->Velocity = Dir * Velocidad;
    ProjectileMovement->SetActive(true);
    ProjectileMovement->UpdateComponentVelocity();
    SetActorRotation(Dir.Rotation());
}

void ACentCosmosProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor == nullptr || OtherActor == this || OtherActor == GetOwner())
        return;

    if (OtherComp != nullptr && OtherComp->IsSimulatingPhysics())
        OtherComp->AddImpulseAtLocation(GetVelocity() * 20.f, GetActorLocation());

    AEnjambre* EnemigoImpactado = Cast<AEnjambre>(OtherActor);
    if (EnemigoImpactado)
    {
        EnemigoImpactado->RecibirDanioEnemigo(Danio);
        Destroy();
        return;
    }

    AEExclusivo* EnemigoExclusivo = Cast<AEExclusivo>(OtherActor);
    if (EnemigoExclusivo)
    {
        EnemigoExclusivo->RecibirDanoEnemigo(Danio); 
        Destroy(); return;
    }

    ATribunalBase* Tribunal = Cast<ATribunalBase>(OtherActor);
    if (Tribunal)
    {
        Tribunal->RecibirDanioJefe(Danio); 
        Destroy(); return;
    }

    AEscudo* EscudoImpactado = Cast<AEscudo>(OtherActor);
    if (EscudoImpactado != nullptr || OtherActor->GetName().Contains(TEXT("Escudo")))
    {
        if (EscudoImpactado == nullptr) EscudoImpactado = Cast<AEscudo>(OtherActor);
        if (EscudoImpactado != nullptr) EscudoImpactado->RecibirDanoEscudo(Danio);
        Destroy();
        return;
    }

    APlacaMetal* Placa = Cast<APlacaMetal>(OtherActor);
    if (Placa)
    {
        Placa->RecibirDanioPlaca(Danio); // Usa DanoBase en el Proyectil de Carga
        Destroy(); // La bala se destruye al chocar con la placa
        return;
    }

    AGrietaAntimateria* GrietaImpactada = Cast<AGrietaAntimateria>(OtherActor);
    if (GrietaImpactada != nullptr)
    {
        GrietaImpactada->ProcesarImpacto();
        Destroy();
        return;
    }

    Destroy();
}