// Fill out your copyright notice in the Description page of Project Settings.

#include "EstadoNivel2.h"
#include "GestorNiveles.h"
#include "UObject/ConstructorHelpers.h"

AEstadoNivel2::AEstadoNivel2()
{
	static ConstructorHelpers::FObjectFinder<USoundBase> ObjetoMusica(TEXT("SoundWave'/Game/Assetssss/A14/Music_Nivel2.Music_Nivel2'"));
	if (ObjetoMusica.Succeeded())
	{
		MusicaNivel = ObjetoMusica.Object;
	}
}

void AEstadoNivel2::IniciarNivel(AGestorNiveles* Gestor)
{
	if (Gestor && Gestor->AudioComponentCentral && MusicaNivel)
	{
		Gestor->AudioComponentCentral->Stop();
		Gestor->AudioComponentCentral->SetSound(MusicaNivel);
		Gestor->AudioComponentCentral->Play();
	}
	UE_LOG(LogTemp, Warning, TEXT("[Nivel 2] Estado Activado: Cargando la musica del Nivel 2."));
}

void AEstadoNivel2::EjecutarTick(float DeltaTime) {}

