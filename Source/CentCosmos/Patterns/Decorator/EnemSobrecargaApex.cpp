// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemSobrecargaApex.h"

float UEnemSobrecargaApex::GetVelocidad(float BaseSpeed) const
{
	return UEnemDecorador::GetVelocidad(BaseSpeed) * 1.5f;
}

float UEnemSobrecargaApex::GetCadencia(float BaseRate) const
{
	return UEnemDecorador::GetCadencia(BaseRate) * 0.5f;
}