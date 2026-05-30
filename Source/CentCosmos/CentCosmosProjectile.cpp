// Copyright Epic Games, Inc. All Rights Reserve

#include "CentCosmosProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Escudo.h"
#include "GrietaAntimateria.h"
#include "Enjambre.h" // <-- IMPORTANTE: Para dañar al enjambre

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
    Danio = 1.0f;
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

void ACentCosmosProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor != nullptr && OtherActor != this)
    {
        AEnjambre* EnemigoImpactado = Cast<AEnjambre>(OtherActor);
        if (EnemigoImpactado)
        {
            EnemigoImpactado->RecibirDanioEnemigo(Danio);
            Destroy();
            return;
        }

        AEscudo* EscudoImpactado = Cast<AEscudo>(OtherActor);
        if (EscudoImpactado != nullptr || OtherActor->GetName().Contains(TEXT("Escudo")))
        {
            if (EscudoImpactado == nullptr) EscudoImpactado = Cast<AEscudo>(OtherActor);
            if (EscudoImpactado != nullptr) EscudoImpactado->RecibirDanoEscudo(Danio);
            Destroy();
            return;
        }

        AGrietaAntimateria* GrietaImpactada = Cast<AGrietaAntimateria>(OtherActor);
        if (GrietaImpactada != nullptr)
        {
            GrietaImpactada->ProcesarImpacto();
            Destroy();
            return;
        }
    }

    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
    {
        OtherComp->AddImpulseAtLocation(GetVelocity() * 20.f, GetActorLocation());
    }

    Destroy();
}