// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemSobrecargaApex.h"
#include "CentCosmosPawn.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

void UEnemSobrecargaApex::AplicarEfecto(ACentCosmosPawn* Nave)
{
	if (!Nave) return;

	Nave->bTieneSobreCargaApex = true;
	Nave->MoveSpeed = Nave->MoveSpeedBase * 1.5f;
	Nave->FireRate = Nave->FireRateBase * 0.5f;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
			TEXT("SobreCarga Apex activada!"));

	FTimerHandle Handle;
	Nave->GetWorldTimerManager().SetTimer(Handle,
		FTimerDelegate::CreateLambda([Nave]()
			{
				if (!IsValid(Nave)) return;
				Nave->DesactivarSobreCargaApex();
			}),
		DuracionEfecto, false);
}