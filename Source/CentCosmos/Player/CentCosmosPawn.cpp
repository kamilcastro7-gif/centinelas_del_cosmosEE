// Copyright Epic Games, Inc. All Rights Reserved.
#include "CentCosmosPawn.h"
#include "CentCosmosProjectile.h"
#include "ProyectilBoomerang.h"
#include "ProyectilCarga.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Observer/Subject.h"

const FName ACentCosmosPawn::MoveForwardBinding("MoveForward");
const FName ACentCosmosPawn::MoveRightBinding("MoveRight");

ACentCosmosPawn::ACentCosmosPawn()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/StarterContent/Shapes/Meshy_AI_Nebula_Vanguard_0525182523_generate.Meshy_AI_Nebula_Vanguard_0525182523_generate"));
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	ShipMeshComponent->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 2200.f;
	CameraBoom->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	MoveSpeed = 1000.0f;
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;

	ArmaActual = ETipoArma::Normal;
	bBoomerangEnVuelo = false;
	TiempoCargaAcumulado = 0.0f;
	bEstaCargando = false;
	ProyectilCargaActual = nullptr;
}

void ACentCosmosPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
}

// Helper para obtener la rotacion real de disparo (compensa el +180 del movimiento)
static FRotator GetFireRotation(const ACentCosmosPawn* Pawn)
{
	FRotator R = Pawn->GetActorRotation();
	R.Yaw -= 180.f;
	return R;
}

void ACentCosmosPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	if (Movement.SizeSquared() > 0.0f)
	{
		FRotator NewRotation = Movement.Rotation();
		NewRotation.Yaw += 180.f; // Compensar orientacion del mesh
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		if (PC->WasInputKeyJustPressed(EKeys::One))   ArmaActual = ETipoArma::Normal;
		else if (PC->WasInputKeyJustPressed(EKeys::Two))   ArmaActual = ETipoArma::Boomerang;
		else if (PC->WasInputKeyJustPressed(EKeys::Three)) ArmaActual = ETipoArma::Carga;
	}

	if (PC && PC->IsInputKeyDown(EKeys::SpaceBar))
	{
		if (ArmaActual == ETipoArma::Carga)
		{
			if (bCanFire)
			{
				if (!bEstaCargando)
				{
					bEstaCargando = true;
					TiempoCargaAcumulado = 0.0f;

					// Usar rotacion corregida para el spawn
					const FRotator FireRotation = GetFireRotation(this);
					const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

					FActorSpawnParameters CargaSpawnParams;
					CargaSpawnParams.Owner = this;
					CargaSpawnParams.Instigator = GetInstigator();
					CargaSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

					UWorld* const World = GetWorld();
					if (World)
					{
						ProyectilCargaActual = World->SpawnActor<AProyectilCarga>(SpawnLocation, FireRotation, CargaSpawnParams);
						if (ProyectilCargaActual)
						{
							ProyectilCargaActual->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
						}
					}
				}
			}

			if (ProyectilCargaActual)
			{
				TiempoCargaAcumulado += DeltaSeconds;
				ProyectilCargaActual->InicializarCarga(TiempoCargaAcumulado);
			}
		}
		else
		{
			// Usar el vector opuesto al forward (que esta invertido por el +180)
			const FVector FireDirection = -GetActorForwardVector();
			FireShot(FireDirection);
		}
	}
	else if (bEstaCargando)
	{
		if (ProyectilCargaActual)
		{
			// Tambien corregir la direccion de liberacion
			const FVector DireccionDeDisparoLimpia = -GetActorForwardVector();
			ProyectilCargaActual->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			ProyectilCargaActual->LiberarProyectil(TiempoCargaAcumulado, DireccionDeDisparoLimpia);
			ProyectilCargaActual = nullptr;

			bCanFire = false;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ACentCosmosPawn::ShotTimerExpired, FireRate);

			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}
		}

		bEstaCargando = false;
		TiempoCargaAcumulado = 0.0f;
	}
}

void ACentCosmosPawn::FireShot(FVector FireDirection)
{
	if (bCanFire == true && FireDirection.SizeSquared() > 0.0f)
	{
		// Rotacion corregida para spawn y direccion de proyectil
		const FRotator FireRotation = GetFireRotation(this);
		const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			if (ArmaActual == ETipoArma::Normal)
			{
				World->SpawnActor<ACentCosmosProjectile>(SpawnLocation, FireRotation, SpawnParams);
			}
			else if (ArmaActual == ETipoArma::Boomerang)
			{
				if (!bBoomerangEnVuelo)
				{
					bBoomerangEnVuelo = true;
					AProyectilBoomerang* Boomerang = World->SpawnActor<AProyectilBoomerang>(SpawnLocation, FireRotation, SpawnParams);
					if (Boomerang)
					{
						Boomerang->NaveDueno = this;
					}
				}
				else
				{
					return;
				}
			}
		}

		bCanFire = false;
		World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ACentCosmosPawn::ShotTimerExpired, FireRate);

		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
	}
}

void ACentCosmosPawn::ShotTimerExpired()
{
	bCanFire = true;
}