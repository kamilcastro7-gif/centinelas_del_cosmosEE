// Fill out your copyright notice in the Description page of Project Settings.

#include "GestorNiveles.h"
#include "EstadoNivel1.h"

AGestorNiveles::AGestorNiveles()
{
	PrimaryActorTick.bCanEverTick = true;

	AudioComponentCentral = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentCentral"));
	AudioComponentCentral->bAutoActivate = false;
}

void AGestorNiveles::BeginPlay()
{
	Super::BeginPlay();

	UWorld* const Mundo = GetWorld();
	if (!Mundo) return;

	// Solo spawneamos el Nivel 1 por el momento
	EstadoNivel1 = Mundo->SpawnActor<AEstadoNivel1>(AEstadoNivel1::StaticClass());

	// Arrancamos el juego automáticamente en el Nivel 1
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
	if (NuevoEstado)
	{
		EstadoActual = NuevoEstado;
		EstadoActual->IniciarNivel(this);
	}
}

