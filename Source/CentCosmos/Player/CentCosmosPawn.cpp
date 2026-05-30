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
#include "GameFramework/ProjectileMovementComponent.h"
#include "Templates/SubclassOf.h"

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

	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(
		TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
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
	MoveSpeedBase = 1000.0f;
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	FireRateBase = 0.1f;
	bCanFire = true;

	ArmaActual = ETipoArma::Normal;
	bBoomerangEnVuelo = false;
	TiempoCargaAcumulado = 0.0f;
	bEstaCargando = false;
	ProyectilCargaActual = nullptr;

	bTieneDisparoTriple = false;
	bTieneSobreCargaApex = false;

	ClaseNormalBP = ACentCosmosProjectile::StaticClass();
	ClaseCargaBP = AProyectilCarga::StaticClass();

	// INICIALIZACION DE VIDA
	VidaMax = 100.f;
	VidaActual = VidaMax;
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
		if (PC->WasInputKeyJustPressed(EKeys::One)) ArmaActual = ETipoArma::Normal;
		else if (PC->WasInputKeyJustPressed(EKeys::Two)) ArmaActual = ETipoArma::Boomerang;
		else if (PC->WasInputKeyJustPressed(EKeys::Three)) ArmaActual = ETipoArma::Carga;
	}

	// Unificado a bCanFire en lugar de !bPuedeDisparar para evitar conflictos de variables
	if (!bCanFire && ArmaActual != ETipoArma::Carga)
	{
		// Solo detenemos la ejecucion si no puede disparar y NO esta usando el arma de carga 
		// (porque la de carga necesita seguir acumulando tiempo en el tick aunque bCanFire sea false)
		return;
	}

	UWorld* const World = GetWorld();

	if (PC && PC->IsInputKeyDown(EKeys::SpaceBar))
	{
		if (World != nullptr)
		{
			const FRotator FireRotation = GetFireRotation(this);
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			if (ArmaActual == ETipoArma::Normal)
			{
				if (bCanFire)
				{
					TSubclassOf<ACentCosmosProjectile> ClaseAUsar = ClaseNormalBP ? ClaseNormalBP : TSubclassOf<ACentCosmosProjectile>(ACentCosmosProjectile::StaticClass());

					FActorSpawnParameters SpawnParamsNormal;
					SpawnParamsNormal.Owner = this;
					SpawnParamsNormal.Instigator = GetInstigator();
					SpawnParamsNormal.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					// Spawneamos el proyectil Normal
					World->SpawnActor<ACentCosmosProjectile>(ClaseAUsar, SpawnLocation, FireRotation, SpawnParamsNormal);

					// Reseteamos el disparo
					bCanFire = false;
					World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ACentCosmosPawn::ShotTimerExpired, FireRate);

					if (FireSound != nullptr)
					{
						UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
					}
				}
			}
			else if (ArmaActual == ETipoArma::Boomerang)
			{
				if (bCanFire && !bBoomerangEnVuelo)
				{
					bBoomerangEnVuelo = true;

					FActorSpawnParameters SpawnParamsBoom;
					SpawnParamsBoom.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					AProyectilBoomerang* Boomerang = World->SpawnActor<AProyectilBoomerang>(AProyectilBoomerang::StaticClass(), SpawnLocation, FireRotation, SpawnParamsBoom);
					if (Boomerang) Boomerang->NaveDueno = this;

					bCanFire = false;
					World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ACentCosmosPawn::ShotTimerExpired, FireRate);

					if (FireSound != nullptr)
					{
						UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
					}
				}
			}
			else if (ArmaActual == ETipoArma::Carga)
			{
				if (bCanFire)
				{
					if (!bEstaCargando)
					{
						bEstaCargando = true;
						TiempoCargaAcumulado = 0.0f;

						TSubclassOf<AProyectilCarga> ClaseCargaAUsar = ClaseCargaBP ? ClaseCargaBP : TSubclassOf<AProyectilCarga>(AProyectilCarga::StaticClass());
						ProyectilCargaActual = World->SpawnActor<AProyectilCarga>(ClaseCargaAUsar, SpawnLocation, FireRotation);
						if (ProyectilCargaActual)
						{
							ProyectilCargaActual->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
						}
					}
				}
			}

			if (ProyectilCargaActual && bEstaCargando)
			{
				TiempoCargaAcumulado += DeltaSeconds;
				ProyectilCargaActual->InicializarCarga(TiempoCargaAcumulado);
			}
		}
		else
		{
			const FVector FireDirection = -GetActorForwardVector();
			FireShot(FireDirection);
		}
	}
	else if (bEstaCargando)
	{
		if (ProyectilCargaActual)
		{
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

void ACentCosmosPawn::FireShot(FVector FireDirection) {}
void ACentCosmosPawn::ShotTimerExpired() { bCanFire = true; }

void ACentCosmosPawn::DesactivarDisparoTriple()
{
	// Limpiamos esta funcion para que solo desactive el power-up
	// y no intente disparar proyectiles por error
	bTieneDisparoTriple = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Disparo Triple Terminado."));
}

void ACentCosmosPawn::DesactivarSobreCargaApex()
{
	bTieneSobreCargaApex = false;
	MoveSpeed = MoveSpeedBase;
	FireRate = FireRateBase;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SobreCarga Apex Terminada."));
}

void ACentCosmosPawn::RecibirDanioNave(float Cantidad)
{
	VidaActual -= Cantidad;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Nave recibio %f de dano. Vida restante: %f"), Cantidad, VidaActual));

	if (VidaActual <= 0.f)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LA NAVE HA SIDO DESTRUIDA!"));
		Destroy();
	}
}