// Fill out your copyright notice in the Description page of Project Settings.

#include "PlacaMetal.h"
#include "TribAsedio.h" 
#include "UObject/ConstructorHelpers.h"
#include "Player/CentCosmosPawn.h"

APlacaMetal::APlacaMetal()
{
	PrimaryActorTick.bCanEverTick = false;

	MallaPlaca = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPlaca"));
	RootComponent = MallaPlaca;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("StaticMesh'/Game/Assetssss/A11/Meshy_AI_Cosmic_Portal_Disk_0607221525_texture.Meshy_AI_Cosmic_Portal_Disk_0607221525_texture'"));
	if (SphereAsset.Succeeded())
	{
		MallaPlaca->SetStaticMesh(SphereAsset.Object);
	}

	MallaPlaca->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	MallaPlaca->SetGenerateOverlapEvents(true);
	MallaPlaca->SetRelativeScale3D(FVector(2.2f, 2.2f, 2.2f));

	VidaMax = 10.f;
	VidaActual = VidaMax;
}

void APlacaMetal::BeginPlay()
{
	Super::BeginPlay();
	VidaActual = VidaMax;
}

void APlacaMetal::RecibirDanioPlaca(float CantidadDanio)
{
	VidaActual -= CantidadDanio;

	if (VidaActual <= 0.f)
	{
		// === BLINDAJE: Solo le avisa al jefe si el jefe sigue vivo ===
		if (IsValid(OwnerJefe))
		{
			ATribAsedio* Jefe = Cast<ATribAsedio>(OwnerJefe);
			if (Jefe)
			{
				Jefe->EliminarPlacaDeArreglo(this);
			}
		}

		Destroy();
	}
}

void APlacaMetal::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
	{
		Cast<ACentCosmosPawn>(OtherActor)->RecibirDanioNave(10.0f);
	}
}

