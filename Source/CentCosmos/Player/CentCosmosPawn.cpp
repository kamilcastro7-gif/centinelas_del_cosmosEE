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
#include "Patterns/Decorator/EnemDecorador.h"
#include "Patterns/Decorator/EnemBaseComp.h"
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

	// Inicializamos la interfaz en nulo, se construirá dinámicamente en BeginPlay
	Atributos = nullptr;

	SubjectVida = nullptr;
	ObservadorVida = nullptr;
}

void ACentCosmosPawn::BeginPlay()
{
	Super::BeginPlay();

	// CONEXIÓN MAESTRA DECORATOR: Instanciamos el componente de salud concreto base
	UEnemBaseComp* Base = NewObject<UEnemBaseComp>(this);
	Base->Inicializar(VidaMax);
	Atributos = Base; // Definimos el inicio de la cadena del patrón

	// CONEXIÓN CON EL OBSERVADOR (Pristina e intacta)
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

	// =========================================================================
	// CONSULTA DINÁMICA AL DECORADOR (Refactoring Guru puro)
	// Solicitamos las variables finales calculadas a lo largo de toda la cadena enlazada
	// =========================================================================
	float VelocidadCalculada = Atributos ? Atributos->GetVelocidad(MoveSpeedBase) : MoveSpeedBase;
	float CadenciaCalculada = Atributos ? Atributos->GetCadencia(FireRateBase) : FireRateBase;
	bool bDisparoTripleActivo = Atributos ? Atributos->GetDisparoTriple() : false;

	// Sincronizamos las variables globales para mantener compatibilidad con el resto del juego
	MoveSpeed = VelocidadCalculada;
	FireRate = CadenciaCalculada;
	bTieneDisparoTriple = bDisparoTripleActivo;
	// =========================================================================

	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

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

	if (!bPuedeDisparar)
	{
		if (bEstaCargando && ProyectilCargaActual)
		{
			ProyectilCargaActual->Destroy();
			ProyectilCargaActual = nullptr;
			bEstaCargando = false;
			TiempoCargaAcumulado = 0.0f;
		}
		return;
	}

	if (!bCanFire && ArmaActual != ETipoArma::Carga) return;

	UWorld* const World = GetWorld();

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

				// Usamos la evaluación limpia obtenida desde el decorador
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
	if (!Atributos) return;

	// Pasamos el daño a través de la cadena lógica del Decorator
	Atributos->RecibirDanio(Cantidad);
	VidaActual = Atributos->GetVida();

	// NOTIFICACIÓN AL OBSERVER: Informamos al sujeto que la vida cambió de forma limpia
	if (SubjectVida)
		SubjectVida->NotifyObservers(FName("VidaActualizada"), VidaActual);

	if (!Atributos->EstaVivo())
	{
		if (SubjectVida && ObservadorVida)
			SubjectVida->RemoveObserver(ObservadorVida);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
				TEXT("LA NAVE HA SIDO DESTRUIDA!"));
		Destroy();
	}
}

void ACentCosmosPawn::RestaurarVida(float Cantidad)
{
	if (!Atributos) return;

	// Aplicamos la curación a través del decorador
	Atributos->RegenerarVida(Cantidad);
	VidaActual = Atributos->GetVida();

	// NOTIFICACIÓN AL OBSERVER
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

void ACentCosmosPawn::FireShot(FVector FireDirection) {}

void ACentCosmosPawn::ShotTimerExpired() { bCanFire = true; }

// Dejamos estas dos funciones obsoletas vacías para evitar roturas con otros sistemas externos
void ACentCosmosPawn::DesactivarDisparoTriple() {}
void ACentCosmosPawn::DesactivarSobreCargaApex() {}

// =========================================================================
// IMPLEMENTACIÓN DE MÉTODOS EJECUTIVOS ENLAZADOS (PATRÓN DECORATOR PURO)
// =========================================================================
void ACentCosmosPawn::AgregarDecorador(UEnemDecorador* NuevoDecorador)
{
	if (NuevoDecorador)
	{
		// El nuevo decorador envuelve la raíz actual de nuestra cadena
		NuevoDecorador->Envolver(Atributos);
		// Actualizamos la raíz apuntando al nuevo envoltorio exterior
		Atributos = NuevoDecorador;
	}
}

void ACentCosmosPawn::RemoverDecorador(UEnemDecorador* DecoradorARemover)
{
	if (!Atributos || !DecoradorARemover) return;

	// Caso A: Si el decorador que expira es la capa más externa de la cadena
	if (Atributos.GetObject() == DecoradorARemover)
	{
		Atributos = DecoradorARemover->ObtenerInner();
		return;
	}

	// Caso B: Si está en medio de la cadena, la recorremos y reconectamos los eslabones
	UObject* Actual = Atributos.GetObject();
	while (Actual)
	{
		UEnemDecorador* DecActual = Cast<UEnemDecorador>(Actual);
		if (DecActual && DecActual->ObtenerInner().GetObject() == DecoradorARemover)
		{
			// El decorador actual se salta el eslabón eliminado enlazándose directo al de abajo
			DecActual->Envolver(DecoradorARemover->ObtenerInner());
			break;
		}
		Actual = DecActual ? DecActual->ObtenerInner().GetObject() : nullptr;
	}
}