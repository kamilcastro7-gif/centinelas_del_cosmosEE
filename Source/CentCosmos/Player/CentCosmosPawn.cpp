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
#include "Observer/VidaObserver.h"
#include "Decorator/EnemDecorador.h"
#include "Decorator/EnemBaseComp.h"
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

	ClaseNormal = ACentCosmosProjectile::StaticClass();
	ClaseCarga = AProyectilCarga::StaticClass();

	VidaMax = 100.f;
	VidaActual = VidaMax;
	Decorador = CreateDefaultSubobject<UEnemDecorador>(TEXT("Decorador"));

	SubjectVida = nullptr;
	ObservadorVida = nullptr;
}

void ACentCosmosPawn::BeginPlay()
{
	Super::BeginPlay();


	// Implementacion del patrón Decorator para el sistema de salud
	UEnemBaseComp* Base = NewObject<UEnemBaseComp>(this);
	Base->Inicializar(VidaMax);
	Decorador->Envolver(TScriptInterface<IEnemigo>(Base));

	if (!SubjectVida)
	{
		SubjectVida = GetWorld()->SpawnActor<ASubject>();
	}
	if (!ObservadorVida)
	{
		ObservadorVida = NewObject<UVidaObserver>();
	}
	if (SubjectVida && ObservadorVida)
	{
		SubjectVida->AddObserver(ObservadorVida);
	}
}

void ACentCosmosPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
}

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

	// --- LÓGICA DE MOVIMIENTO (No se ve afectada por el silencio) ---
	if (Movement.SizeSquared() > 0.0f)
	{
		FRotator NewRotation = Movement.Rotation();
		NewRotation.Yaw += 180.f;
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
		if (PC->WasInputKeyJustPressed(EKeys::One))       ArmaActual = ETipoArma::Normal;
		else if (PC->WasInputKeyJustPressed(EKeys::Two))  ArmaActual = ETipoArma::Boomerang;
		else if (PC->WasInputKeyJustPressed(EKeys::Three))ArmaActual = ETipoArma::Carga;
	}

	// =========================================================================
	// --- NUEVO: FILTRO DE SILENCIO (ECLIPSE SILENCIOSO) ---
	// Si el enemigo nos silencia, cancelamos las cargas activas y bloqueamos el disparo
	if (!bPuedeDisparar)
	{
		if (bEstaCargando && ProyectilCargaActual)
		{
			ProyectilCargaActual->Destroy(); // Destruye la energía a medio cargar
			ProyectilCargaActual = nullptr;
			bEstaCargando = false;
			TiempoCargaAcumulado = 0.0f;
		}
		return; // ¡El return evita que la nave lea el código de disparo de abajo!
	}
	// =========================================================================

	if (!bCanFire && ArmaActual != ETipoArma::Carga) return;

	UWorld* const World = GetWorld();

	// --- LÓGICA DE DISPARO ---
	if (PC && PC->IsInputKeyDown(EKeys::SpaceBar))
	{
		if (World != nullptr)
		{
			const FRotator FireRotation = GetFireRotation(this);
			const FVector  SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			if (ArmaActual == ETipoArma::Normal && bCanFire)
			{
				TSubclassOf<ACentCosmosProjectile> ClaseAUsar = ClaseNormal
					? ClaseNormal
					: TSubclassOf<ACentCosmosProjectile>(ACentCosmosProjectile::StaticClass());

				FActorSpawnParameters Params;
				Params.Owner = this;
				Params.Instigator = GetInstigator();
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				if (bTieneDisparoTriple)
				{
					World->SpawnActor<ACentCosmosProjectile>(ClaseAUsar, SpawnLocation, FireRotation, Params);

					FRotator RotIzq = FireRotation;
					RotIzq.Yaw -= 20.f;
					FVector SpawnIzq = SpawnLocation + FRotator(0.f, FireRotation.Yaw - 90.f, 0.f).Vector() * 40.f;
					ACentCosmosProjectile* PIzq = World->SpawnActor<ACentCosmosProjectile>(ClaseAUsar, SpawnIzq, RotIzq, Params);
					if (PIzq) PIzq->ForzarDireccion(RotIzq.Vector(), 3000.f);

					FRotator RotDer = FireRotation;
					RotDer.Yaw += 20.f;
					FVector SpawnDer = SpawnLocation + FRotator(0.f, FireRotation.Yaw + 90.f, 0.f).Vector() * 40.f;
					ACentCosmosProjectile* PDer = World->SpawnActor<ACentCosmosProjectile>(ClaseAUsar, SpawnDer, RotDer, Params);
					if (PDer) PDer->ForzarDireccion(RotDer.Vector(), 3000.f);
				}
				else
				{
					World->SpawnActor<ACentCosmosProjectile>(ClaseAUsar, SpawnLocation, FireRotation, Params);
				}

				bCanFire = false;
				World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ACentCosmosPawn::ShotTimerExpired, FireRate);

				if (FireSound) UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}
			else if (ArmaActual == ETipoArma::Boomerang && bCanFire && !bBoomerangEnVuelo)
			{
				bBoomerangEnVuelo = true;

				FActorSpawnParameters SpawnParamsBoom;
				SpawnParamsBoom.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AProyectilBoomerang* Boomerang = World->SpawnActor<AProyectilBoomerang>(
					AProyectilBoomerang::StaticClass(), SpawnLocation, FireRotation, SpawnParamsBoom);
				if (Boomerang) Boomerang->NaveDueno = this;

				bCanFire = false;
				World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ACentCosmosPawn::ShotTimerExpired, FireRate);

				if (FireSound) UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}
			else if (ArmaActual == ETipoArma::Carga && bCanFire)
			{
				if (!bEstaCargando)
				{
					bEstaCargando = true;
					TiempoCargaAcumulado = 0.0f;

					TSubclassOf<AProyectilCarga> ClaseCargaAUsar = ClaseCarga
						? ClaseCarga
						: TSubclassOf<AProyectilCarga>(AProyectilCarga::StaticClass());

					ProyectilCargaActual = World->SpawnActor<AProyectilCarga>(ClaseCargaAUsar, SpawnLocation, FireRotation);
					if (ProyectilCargaActual)
						ProyectilCargaActual->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
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
			FireShot(-GetActorForwardVector());
		}
	}
	else if (bEstaCargando)
	{
		if (ProyectilCargaActual)
		{
			const FVector Dir = -GetActorForwardVector();
			ProyectilCargaActual->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			ProyectilCargaActual->LiberarProyectil(TiempoCargaAcumulado, Dir);
			ProyectilCargaActual = nullptr;

			bCanFire = false;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ACentCosmosPawn::ShotTimerExpired, FireRate);
		}

		if (FireSound) UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());

		bEstaCargando = false;
		TiempoCargaAcumulado = 0.0f;
	}
}

void ACentCosmosPawn::RecibirDanioNave(float Cantidad)
{
	if (!Decorador) return;

	Decorador->RecibirDanio(Cantidad);
	VidaActual = Decorador->GetVida();

	if (SubjectVida)
		SubjectVida->NotifyObservers(FName("VidaActualizada"), VidaActual);

	if (!Decorador->EstaVivo())
	{
		// Detach del observer antes de destruir — igual que RemoveMeFromTheList()
		if (SubjectVida && ObservadorVida)
			SubjectVida->RemoveObserver(ObservadorVida);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
				TEXT("LA NAVE HA SIDO DESTRUIDA!"));
		Destroy();
	}
}

void ACentCosmosPawn::FireShot(FVector FireDirection) {}

void ACentCosmosPawn::ShotTimerExpired() { bCanFire = true; }

void ACentCosmosPawn::DesactivarDisparoTriple()
{
	bTieneDisparoTriple = false;
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Disparo Triple Terminado."));
}

void ACentCosmosPawn::DesactivarSobreCargaApex()
{
	bTieneSobreCargaApex = false;
	MoveSpeed = MoveSpeedBase;
	FireRate = FireRateBase;
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SobreCarga Apex Terminada."));
}

void ACentCosmosPawn::RestaurarVida(float Cantidad)
{
	if (!Decorador) return;

	// El Decorator regula que no pase de VidaMaxima
	Decorador->RegenerarVida(Cantidad);
	VidaActual = Decorador->GetVida();

	// Notificar al Observer para que actualice el HUD
	if (SubjectVida)
	{
		SubjectVida->NotifyObservers(FName("VidaActualizada"), VidaActual);
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, 
			TEXT("Vida Restaurada!"));
	}
}