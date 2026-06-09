// Fill out your copyright notice in the Description page of Project Settings.

#include "EstadoNivel4.h"
#include "GestorNiveles.h"
#include "UObject/ConstructorHelpers.h"

AEstadoNivel4::AEstadoNivel4()
{
	static ConstructorHelpers::FObjectFinder<USoundBase> ObjetoMusica(TEXT("SoundWave'/Game/Assetssss/A14/Music_Nivel4.Music_Nivel4'"));
	if (ObjetoMusica.Succeeded())
	{
		MusicaNivel = ObjetoMusica.Object;
	}
}

void AEstadoNivel4::IniciarNivel(AGestorNiveles* Gestor)
{
	if (Gestor && Gestor->AudioComponentCentral && MusicaNivel)
	{
		Gestor->AudioComponentCentral->Stop();
		Gestor->AudioComponentCentral->SetSound(MusicaNivel);
		Gestor->AudioComponentCentral->Play();
	}
	UE_LOG(LogTemp, Warning, TEXT("[Nivel 4] Estado Activado: Cargando la musica del Nivel 4."));
}

void AEstadoNivel4::EjecutarTick(float DeltaTime) {}
