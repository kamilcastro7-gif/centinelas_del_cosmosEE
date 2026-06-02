// Fill out your copyright notice in the Description page of Project Settings.
#include "NivelFacilBuilder.h"
#include "Engine/World.h"
#include "EnemyFactory.h"
#include "GeneradorAmbientacion.h"
#include "TribunalVigilante.h"
#include "Engine/Engine.h"
#include "ArenaCombate.h"
#include "ObstaculoNave.h"
#include "ObstaculoSatelite.h"
#include "ObstaculoRestos.h"
#include "Kismet/GameplayStatics.h"

ANivelFacilBuilder::ANivelFacilBuilder()
{
	PrimaryActorTick.bCanEverTick = true;
	bBossSpawneado = false;
}

void ANivelFacilBuilder::Reset()
{
	NivelActual = FNivel();
	EnemigosGenerados.Empty();
	EnemigosOla1.Empty();
	bBossSpawneado = false;
}

void ANivelFacilBuilder::SetMetadatos(const FString& Nombre, float TiempoLimite)
{
	NivelActual.Nombre = Nombre;
	NivelActual.TiempoLimite = TiempoLimite;
}

void ANivelFacilBuilder::SetDificultad(float Dificultad)
{
	NivelActual.Dificultad = Dificultad;
}

void ANivelFacilBuilder::AgregarAmbientacion(UWorld* World)
{
	if (!World) return;

	// Delegamos la ambientacion al GeneradorAmbientacion (AbstractFactory)
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AGeneradorAmbientacion* Generador = World->SpawnActor<AGeneradorAmbientacion>(
		AGeneradorAmbientacion::StaticClass(),
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		Params);

	// BeginPlay del generador ya spawna los 30 obstaculos automaticamente
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, TEXT("[NivelFacil] Ambientacion generada."));
}

void ANivelFacilBuilder::AgregarEnemigos(UWorld* World)
{
	if (!World) return;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawneamos la factory — es un AActor que necesita existir en el mundo
	AEnemyFactory* Factory = World->SpawnActor<AEnemyFactory>(
		AEnemyFactory::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

	if (!Factory) return;

	// ---  enemigos ---

	// 5 Vastagos
	for (int32 i = 0; i < 12; i++)
	{
		FVector Pos = FVector(FMath::RandRange(-1000, 1000), FMath::RandRange(-1000, 1000), 150.f);
		AActor* E = Factory->FabricarVastago(Pos, FRotator::ZeroRotator);
		if (E)
		{
			EnemigosGenerados.Add(E);
			EnemigosOla1.Add(E);
		}
	}

	// 5 Vigias
	for (int32 i = 0; i < 2; i++)
	{
		FVector Pos = FVector(FMath::RandRange(-1000, 1000), FMath::RandRange(-1000, 1000), 150.f);
		AActor* E = Factory->FabricarVigia(Pos, FRotator::ZeroRotator);
		if (E)
		{
			EnemigosGenerados.Add(E);
			EnemigosOla1.Add(E);
		}
	}

	// 3 Heraldos
	for (int32 i = 0; i < 1; i++)
	{
		FVector Pos = FVector(FMath::RandRange(-800, 800), FMath::RandRange(-800, 800), 150.f);
		AActor* E = Factory->FabricarHeraldo(Pos, FRotator::ZeroRotator);
		if (E)
		{
			EnemigosGenerados.Add(E);
			EnemigosOla1.Add(E);
		}
	}

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange,
			FString::Printf(TEXT("[NivelFacil] Ola 1 generada: %d enemigos."), EnemigosOla1.Num()));
}

void ANivelFacilBuilder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (bBossSpawneado || EnemigosOla1.Num() == 0) return;

	UWorld* World = GetWorld();
	if (!World) return;

	VerificarYSpawnearBoss(World);
}

void ANivelFacilBuilder::VerificarYSpawnearBoss(UWorld* World)
{
	// Contamos cuántos enemigos de ola 1 siguen vivos
	int32 Vivos = 0;
	for (const TWeakObjectPtr<AActor>& Ref : EnemigosOla1)
	{
		if (Ref.IsValid())
			Vivos++;
	}

	if (Vivos > 0) return;

	// Todos muertos — iniciamos el evento del Jefe
	bBossSpawneado = true;

	// --- 1. ELIMINAR LA AMBIENTACIÓN ---
	TArray<AActor*> ActoresEncontrados;

	// Destruimos Naves
	UGameplayStatics::GetAllActorsOfClass(World, AObstaculoNave::StaticClass(), ActoresEncontrados);
	for (AActor* Obs : ActoresEncontrados) { Obs->Destroy(); }

	// Destruimos Satélites
	UGameplayStatics::GetAllActorsOfClass(World, AObstaculoSatelite::StaticClass(), ActoresEncontrados);
	for (AActor* Obs : ActoresEncontrados) { Obs->Destroy(); }

	// Destruimos Restos
	UGameplayStatics::GetAllActorsOfClass(World, AObstaculoRestos::StaticClass(), ActoresEncontrados);
	for (AActor* Obs : ActoresEncontrados) { Obs->Destroy(); }

	// Destruimos el Generador en sí (por limpieza)
	UGameplayStatics::GetAllActorsOfClass(World, AGeneradorAmbientacion::StaticClass(), ActoresEncontrados);
	for (AActor* Gen : ActoresEncontrados) { Gen->Destroy(); }

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("[NivelFacil] Ambientación eliminada."));


	// --- 2. SPAWNEAR LA ARENA DE COMBATE ---
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Aparecemos la arena en el centro (Z=0 para que el suelo quede alineado)
	FVector PosArena = FVector(0.f, 0.f, 0.f);
	AArenaCombate* Arena = World->SpawnActor<AArenaCombate>(
		AArenaCombate::StaticClass(), PosArena, FRotator::ZeroRotator, Params);


	// --- 3. SPAWNEAR AL JEFE ---
	// Lo elevamos un poco en Z (ej. 250.f) para asegurarnos de que no quede enterrado en el suelo de la arena
	FVector PosBoss = FVector(0.f, 0.f, 250.f);
	ATribunalVigilante* Boss = World->SpawnActor<ATribunalVigilante>(
		ATribunalVigilante::StaticClass(), PosBoss, FRotator::ZeroRotator, Params);

	if (Boss)
	{
		EnemigosGenerados.Add(Boss);
		NivelActual.Enemigos.Add(Boss);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
				TEXT("[NivelFacil] OLA 1 COMPLETADA — ¡Arena y TribunalVigilante aparecen!"));
	}
}
FNivel ANivelFacilBuilder::ObtenerNivel()
{
	NivelActual.Enemigos = EnemigosGenerados;
	return NivelActual;
}