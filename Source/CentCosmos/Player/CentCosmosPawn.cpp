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

	Atributos = nullptr;

	SubjectVida = nullptr;
	ObservadorVida = nullptr;
}

void ACentCosmosPawn::BeginPlay()
{
	Super::BeginPlay();

	UEnemBaseComp* Base = NewObject<UEnemBaseComp>(this);
	Base->Inicializar(VidaMax);
	Atributos = Base;

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

	float VelocidadCalculada = Atributos ? Atributos->GetVelocidad(MoveSpeedBase) : MoveSpeedBase;
	float CadenciaCalculada = Atributos ? Atributos->GetCadencia(FireRateBase) : FireRateBase;
	bool bDisparoTripleActivo = Atributos ? Atributos->GetDisparoTriple() : false;

	MoveSpeed = VelocidadCalculada;
	FireRate = CadenciaCalculada;
	bTieneDisparoTriple = bDisparoTripleActivo;

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

	// =========================================================================
	// LÓGICA DE CONTROL DE TECLAS Y AUTO-DISPARO (Evita el lag por exceso de carga)
	// =========================================================================
	bool bBotonPresionado = PC && PC->IsInputKeyDown(EKeys::SpaceBar);
	bool bAlcanzoLimiteCarga = TiempoCargaAcumulado >= 3.0f; // Tiempo máximo de carga antes de auto-disparar

	// Si presiona el botón Y aún no llega al límite de 3 segundos, seguimos cargando/disparando
	if (bBotonPresionado && !bAlcanzoLimiteCarga)
	{
		if (World != nullptr)
		{
			const FRotator FireRotation = GetFireRotation(this);
			const FVector  SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			if (ArmaActual == ETipoArma::Normal && bCanFire)
			{
				TSubclassOf<ACentCosmosProjectile> ClaseAUsar = ClaseNormal ? ClaseNormal : TSubclassOf<ACentCosmosProjectile>(ACentCosmosProjectile::StaticClass());

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

				if (bTieneDisparoTriple)
				{
					AProyectilBoomerang* BoomCentro = World->SpawnActor<AProyectilBoomerang>(AProyectilBoomerang::StaticClass(), SpawnLocation, FireRotation, SpawnParamsBoom);
					if (BoomCentro) BoomCentro->NaveDueno = this;

					FRotator RotIzq = FireRotation; RotIzq.Yaw -= 20.f;
					AProyectilBoomerang* BoomIzq = World->SpawnActor<AProyectilBoomerang>(AProyectilBoomerang::StaticClass(), SpawnLocation, RotIzq, SpawnParamsBoom);
					if (BoomIzq) BoomIzq->NaveDueno = this;

					FRotator RotDer = FireRotation; RotDer.Yaw += 20.f;
					AProyectilBoomerang* BoomDer = World->SpawnActor<AProyectilBoomerang>(AProyectilBoomerang::StaticClass(), SpawnLocation, RotDer, SpawnParamsBoom);
					if (BoomDer) BoomDer->NaveDueno = this;
				}
				else
				{
					AProyectilBoomerang* Boomerang = World->SpawnActor<AProyectilBoomerang>(AProyectilBoomerang::StaticClass(), SpawnLocation, FireRotation, SpawnParamsBoom);
					if (Boomerang) Boomerang->NaveDueno = this;
				}

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

					TSubclassOf<AProyectilCarga> ClaseCargaAUsar = ClaseCarga ? ClaseCarga : TSubclassOf<AProyectilCarga>(AProyectilCarga::StaticClass());

					ProyectilCargaActual = World->SpawnActor<AProyectilCarga>(ClaseCargaAUsar, SpawnLocation, FireRotation);
					if (ProyectilCargaActual)
					{
						ProyectilCargaActual->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
					}
				}
			}

			if (ProyectilCargaActual && bEstaCargando)
			{
				float FactorCargaApex = (FireRateBase > 0.f && FireRate > 0.f) ? (FireRateBase / FireRate) : 1.0f;
				TiempoCargaAcumulado += (DeltaSeconds * FactorCargaApex);
				ProyectilCargaActual->InicializarCarga(TiempoCargaAcumulado);
			}
		}
		else
		{
			FireShot(-GetActorForwardVector());
		}
	}
	// =========================================================================
	// SOLTAR LA TECLA O AUTO-DISPARO POR LÍMITE DE TIEMPO
	// =========================================================================
	else if (bEstaCargando)
	{
		if (ProyectilCargaActual)
		{
			const FVector DirCentro = -GetActorForwardVector();
			FVector SpawnLoc = ProyectilCargaActual->GetActorLocation();
			FVector EscalaFinalCarga = ProyectilCargaActual->GetActorScale3D();

			ProyectilCargaActual->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			ProyectilCargaActual->LiberarProyectil(TiempoCargaAcumulado, DirCentro);

			bool bEsCargaMaxima = TiempoCargaAcumulado >= 1.5f;

			if (bTieneDisparoTriple && !bEsCargaMaxima)
			{
				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				TSubclassOf<AProyectilCarga> ClaseAUsar = ClaseCarga ? ClaseCarga : TSubclassOf<AProyectilCarga>(AProyectilCarga::StaticClass());

				FRotator RotCentro = DirCentro.Rotation();

				// CLON IZQUIERDO
				FRotator RotIzq = RotCentro;
				RotIzq.Yaw -= 20.f;
				FVector SpawnIzq = SpawnLoc + FRotator(0.f, RotCentro.Yaw - 90.f, 0.f).Vector() * 100.f;

				AProyectilCarga* CargaIzq = GetWorld()->SpawnActor<AProyectilCarga>(ClaseAUsar, SpawnIzq, RotIzq, Params);
				if (CargaIzq)
				{
					CargaIzq->SetActorScale3D(EscalaFinalCarga);
					CargaIzq->InicializarCarga(TiempoCargaAcumulado);
					CargaIzq->LiberarProyectil(TiempoCargaAcumulado, RotIzq.Vector());
				}

				// CLON DERECHO
				FRotator RotDer = RotCentro;
				RotDer.Yaw += 20.f;
				FVector SpawnDer = SpawnLoc + FRotator(0.f, RotCentro.Yaw + 90.f, 0.f).Vector() * 100.f;

				AProyectilCarga* CargaDer = GetWorld()->SpawnActor<AProyectilCarga>(ClaseAUsar, SpawnDer, RotDer, Params);
				if (CargaDer)
				{
					CargaDer->SetActorScale3D(EscalaFinalCarga);
					CargaDer->InicializarCarga(TiempoCargaAcumulado);
					CargaDer->LiberarProyectil(TiempoCargaAcumulado, RotDer.Vector());
				}
			}

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

	Atributos->RecibirDanio(Cantidad);
	VidaActual = Atributos->GetVida();

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

	Atributos->RegenerarVida(Cantidad);
	VidaActual = Atributos->GetVida();

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

void ACentCosmosPawn::DesactivarDisparoTriple() {}
void ACentCosmosPawn::DesactivarSobreCargaApex() {}

void ACentCosmosPawn::AgregarDecorador(UEnemDecorador* NuevoDecorador)
{
	if (NuevoDecorador)
	{
		NuevoDecorador->Envolver(Atributos);
		Atributos = NuevoDecorador;
	}
}

void ACentCosmosPawn::RemoverDecorador(UEnemDecorador* DecoradorARemover)
{
	if (!Atributos || !DecoradorARemover) return;
	if (Atributos.GetObject() == DecoradorARemover)
	{
		Atributos = DecoradorARemover->ObtenerInner();
		return;
	}

	UObject* Actual = Atributos.GetObject();
	while (Actual)
	{
		UEnemDecorador* DecActual = Cast<UEnemDecorador>(Actual);
		if (DecActual && DecActual->ObtenerInner().GetObject() == DecoradorARemover)
		{
			DecActual->Envolver(DecoradorARemover->ObtenerInner());
			break;
		}
		Actual = DecActual ? DecActual->ObtenerInner().GetObject() : nullptr;
	}
}