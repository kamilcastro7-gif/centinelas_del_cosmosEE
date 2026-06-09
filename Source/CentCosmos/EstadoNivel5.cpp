// Fill out your copyright notice in the Description page of Project Settings.

#include "EstadoNivel5.h"
#include "GestorNiveles.h"
#include "UObject/ConstructorHelpers.h"

AEstadoNivel5::AEstadoNivel5()
{
	static ConstructorHelpers::FObjectFinder<USoundBase> ObjetoMusica(TEXT("SoundWave'/Game/Assetssss/A14/Music_Nivel5.Music_Nivel5'"));
	if (ObjetoMusica.Succeeded())
	{
		MusicaNivel = ObjetoMusica.Object;
	}
}

void AEstadoNivel5::IniciarNivel(AGestorNiveles* Gestor)
{
	if (Gestor && Gestor->AudioComponentCentral && MusicaNivel)
	{
		Gestor->AudioComponentCentral->Stop();
		Gestor->AudioComponentCentral->SetSound(MusicaNivel);
		Gestor->AudioComponentCentral->Play();
	}
	UE_LOG(LogTemp, Warning, TEXT("[Nivel 5] Estado Activado: Cargando la musica del Nivel 5."));
}

void AEstadoNivel5::EjecutarTick(float DeltaTime) {}

