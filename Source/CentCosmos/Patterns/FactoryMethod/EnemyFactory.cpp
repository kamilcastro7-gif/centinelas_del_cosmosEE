// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyFactory.h"
#include "EVastago_Del_infierno.h"
#include "EVigia_Del_Vacio.h"
#include "EHeraldo_De_La_Ruina.h"
#include "Tribunal_Ira.h"
#include "TribunalCaos.h"
#include "TribAsedio.h"
#include "TribunalSupremo.h"
#include "TribunalTormenta.h"
#include "TribunalVigilante.h"
#include "Engine/World.h"

AEnemyFactory::AEnemyFactory()
{
	PrimaryActorTick.bCanEverTick = false;
	ClaseVastago = AEVastago_Del_infierno::StaticClass();
	ClaseVigia = AEVigia_Del_Vacio::StaticClass();
	ClaseHeraldo = AEHeraldo_De_La_Ruina::StaticClass();
	ClaseTribunalIra = ATribunal_Ira::StaticClass();
	ClaseTribunalCaos = ATribunalCaos::StaticClass();
	ClaseTribAsedio = ATribAsedio::StaticClass();
	ClaseTribSupremo = ATribunalSupremo::StaticClass();
	ClaseTribTormenta = ATribunalTormenta::StaticClass();
	ClaseTribVigilante = ATribunalVigilante::StaticClass();
}

AActor* AEnemyFactory::FabricarVastago(const FVector& Posicion, const FRotator& Rotacion)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return nullptr;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	return Mundo->SpawnActor<AEVastago_Del_infierno>(ClaseVastago, Posicion, Rotacion, Params);
}

AActor* AEnemyFactory::FabricarVigia(const FVector& Posicion, const FRotator& Rotacion)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return nullptr;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	return Mundo->SpawnActor<AEVigia_Del_Vacio>(ClaseVigia, Posicion, Rotacion, Params);
}

AActor* AEnemyFactory::FabricarHeraldo(const FVector& Posicion, const FRotator& Rotacion)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return nullptr;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	return Mundo->SpawnActor<AEHeraldo_De_La_Ruina>(ClaseHeraldo, Posicion, Rotacion, Params);
}

AActor* AEnemyFactory::FabricarTribunalIra(const FVector& Posicion, const FRotator& Rotacion)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return nullptr;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	return Mundo->SpawnActor<ATribunal_Ira>(ClaseTribunalIra, Posicion, Rotacion, Params);
}

AActor* AEnemyFactory::FabricarTribunalCaos(const FVector& Posicion, const FRotator& Rotacion)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return nullptr;
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return Mundo->SpawnActor<ATribunalCaos>(ClaseTribunalCaos, Posicion, Rotacion, Params);
}

AActor* AEnemyFactory::FabricarTribAsedio(const FVector& Posicion, const FRotator& Rotacion)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return nullptr;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	return Mundo->SpawnActor<ATribAsedio>(ClaseTribAsedio, Posicion, Rotacion, Params);
}

AActor*  AEnemyFactory::FabricarTribunalSupremo(const FVector& Posicion, const FRotator& Rotacion)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return nullptr;
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return Mundo->SpawnActor<ATribunalSupremo>(ClaseTribSupremo, Posicion, Rotacion, Params);
}

AActor* AEnemyFactory::FabricarTribunalTormenta(const FVector& Posicion, const FRotator& Rotacion)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return nullptr;
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return Mundo->SpawnActor<ATribunalTormenta>(ClaseTribTormenta, Posicion, Rotacion, Params);
}

AActor* AEnemyFactory::FabricarTribunalVigilante(const FVector& Posicion, const FRotator& Rotacion)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo) return nullptr;
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return Mundo->SpawnActor<ATribunalVigilante>(ClaseTribVigilante, Posicion, Rotacion, Params);
}