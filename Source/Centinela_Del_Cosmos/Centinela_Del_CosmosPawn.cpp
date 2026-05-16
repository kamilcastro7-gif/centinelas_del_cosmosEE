// Copyright Epic Games, Inc. All Rights Reserved.

#include "Centinela_Del_CosmosPawn.h"
#include "Centinela_Del_CosmosProjectile.h"
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

const FName ACentinela_Del_CosmosPawn::MoveForwardBinding("MoveForward");
const FName ACentinela_Del_CosmosPawn::MoveRightBinding("MoveRight");
const FName ACentinela_Del_CosmosPawn::FireForwardBinding("FireForward");
const FName ACentinela_Del_CosmosPawn::FireRightBinding("FireRight");

ACentinela_Del_CosmosPawn::ACentinela_Del_CosmosPawn()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;

	ArmaActual = ETipoArma::Normal;
	bBoomerangEnVuelo = false;
	TiempoCargaAcumulado = 0.0f;
	bEstaCargando = false;
	ProyectilCargaActual = nullptr;
}

void ACentinela_Del_CosmosPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);

	PlayerInputComponent->BindAction("EspacioDisparo", IE_Pressed, this, &ACentinela_Del_CosmosPawn::ShotTimerExpired);
}

void ACentinela_Del_CosmosPawn::Tick(float DeltaSeconds)
{
	// 1. OBTENER EJES DE MOVIMIENTO (WASD / Joysticks)
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// 2. CÁLCULO DE MOVIMIENTO CONSTANTE AL 100% (No se frena la nave)
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	if (Movement.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = Movement.Rotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}

	// 3. CAMBIO DE ARMAS CON TECLAS 1, 2 Y 3
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		if (PC->WasInputKeyJustPressed(EKeys::One))   ArmaActual = ETipoArma::Normal;
		else if (PC->WasInputKeyJustPressed(EKeys::Two))  ArmaActual = ETipoArma::Boomerang;
		else if (PC->WasInputKeyJustPressed(EKeys::Three)) ArmaActual = ETipoArma::Carga;
	}

	// 4. CONTROL DE DISPARO POR TECLA ESPACIO
	if (PC && PC->IsInputKeyDown(EKeys::SpaceBar))
	{
		if (ArmaActual == ETipoArma::Carga)
		{
			if (bCanFire)
			{
				// Si apenas empezamos a presionar, spawneamos la esfera pegada a la nave
				if (!bEstaCargando)
				{
					bEstaCargando = true;
					TiempoCargaAcumulado = 0.0f;

					const FRotator FireRotation = GetActorRotation();
					const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

					UWorld* const World = GetWorld();
					if (World)
					{
						ProyectilCargaActual = World->SpawnActor<AProyectilCarga>(SpawnLocation, FireRotation);

						// Adjuntamos físicamente la pelota a la nave para evitar que se desoriente al girar
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

				// Va escalando la pelota en base al tiempo retenido
				ProyectilCargaActual->InicializarCarga(TiempoCargaAcumulado);
			}

			// Disparamos ráfagas de proyectiles normales paralelos mientras va cargando la gorda
			if (bCanFire)
			{
				UWorld* const World = GetWorld();
				if (World)
				{
					const FRotator FireRotation = GetActorRotation();
					const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);
					World->SpawnActor<ACentinela_Del_CosmosProjectile>(SpawnLocation, FireRotation);

					bCanFire = false;
					World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ACentinela_Del_CosmosPawn::ShotTimerExpired, FireRate);
				}
			}
		}
		else
		{
			// Disparo normal para las otras armas del inventario
			const FVector FireDirection = GetActorForwardVector();
			FireShot(FireDirection);
		}
	}
	else if (bEstaCargando)
	{
		// AL SOLTAR ESPACIO: Liberamos y disparamos con precisión absoluta hacia adelante
		if (ProyectilCargaActual)
		{
			// Guardamos el vector de hacia dónde mira la nave JUSTO ANTES de desvincularla
			FVector DireccionDeDisparoLimpia = GetActorForwardVector();

			// Desvinculamos el proyectil de la nave para que sea libre en el mundo
			ProyectilCargaActual->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			// Le pasamos la dirección limpia guardada para blindar que NUNCA se quede quieta
			ProyectilCargaActual->LiberarProyectil(TiempoCargaAcumulado, DireccionDeDisparoLimpia);
			ProyectilCargaActual = nullptr;

			// Se activa el cooldown de disparo
			bCanFire = false;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ACentinela_Del_CosmosPawn::ShotTimerExpired, FireRate);

			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}
		}

		bEstaCargando = false;
		TiempoCargaAcumulado = 0.0f;
	}
}

void ACentinela_Del_CosmosPawn::FireShot(FVector FireDirection)
{
	if (bCanFire == true)
	{
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = GetActorRotation();
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				if (ArmaActual == ETipoArma::Normal)
				{
					World->SpawnActor<ACentinela_Del_CosmosProjectile>(SpawnLocation, FireRotation);
				}
				else if (ArmaActual == ETipoArma::Boomerang)
				{
					if (!bBoomerangEnVuelo)
					{
						bBoomerangEnVuelo = true;
						AProyectilBoomerang* Boomerang = World->SpawnActor<AProyectilBoomerang>(SpawnLocation, FireRotation);
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
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ACentinela_Del_CosmosPawn::ShotTimerExpired, FireRate);

			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}
		}
	}
}

void ACentinela_Del_CosmosPawn::ShotTimerExpired()
{
	bCanFire = true;
}

