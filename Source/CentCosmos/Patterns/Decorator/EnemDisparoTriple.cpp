// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemDisparoTriple.h"
#include "CentCosmosPawn.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

void UEnemDisparoTriple::AplicarEfecto(ACentCosmosPawn* Nave)
{
	if (!Nave) return;

	Nave->bTieneDisparoTriple = true;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
			TEXT("Disparo Triple activado!"));

	FTimerHandle Handle;
	Nave->GetWorldTimerManager().SetTimer(Handle,
		FTimerDelegate::CreateLambda([Nave]()
			{
				if (!IsValid(Nave)) return;
				Nave->DesactivarDisparoTriple();
			}),
		DuracionEfecto, false);
}