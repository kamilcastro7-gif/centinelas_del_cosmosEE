// Fill out your copyright notice in the Description page of Project Settings.

#include "GestorNiveles.h"
#include "EstadoNivel1.h"
#include "EstadoNivel2.h"
#include "EstadoNivel3.h"
#include "EstadoNivel4.h"
#include "EstadoNivel5.h"
#include "EstadoNivel6.h"

AGestorNiveles::AGestorNiveles()
{
	PrimaryActorTick.bCanEverTick = true;

	AudioComponentCentral = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentCentral"));
	AudioComponentCentral->bAutoActivate = false;

	EstadoActual = nullptr;
}

void AGestorNiveles::BeginPlay()
{
	Super::BeginPlay();

	UWorld* const Mundo = GetWorld();
	if (!Mundo) return;

	EstadoNivel1 = Mundo->SpawnActor<AEstadoNivel1>(AEstadoNivel1::StaticClass());
	EstadoNivel2 = Mundo->SpawnActor<AEstadoNivel2>(AEstadoNivel2::StaticClass());
	EstadoNivel3 = Mundo->SpawnActor<AEstadoNivel3>(AEstadoNivel3::StaticClass());
	EstadoNivel4 = Mundo->SpawnActor<AEstadoNivel4>(AEstadoNivel4::StaticClass());
	EstadoNivel5 = Mundo->SpawnActor<AEstadoNivel5>(AEstadoNivel5::StaticClass());
	EstadoNivel6 = Mundo->SpawnActor<AEstadoNivel6>(AEstadoNivel6::StaticClass());

	CambiarEstado(EstadoNivel1);
}

void AGestorNiveles::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EstadoActual)
	{
		EstadoActual->EjecutarTick(DeltaTime);
	}
}

void AGestorNiveles::CambiarEstado(IEstadoNivel* NuevoEstado)
{
	if (NuevoEstado && EstadoActual != NuevoEstado)
	{
		EstadoActual = NuevoEstado;
		EstadoActual->IniciarNivel(this);
	}
}

