// Fill out your copyright notice in the Description page of Project Settings.

#include "EstadoNivel6.h"
#include "GestorNiveles.h"
#include "UObject/ConstructorHelpers.h"

AEstadoNivel6::AEstadoNivel6()
{
	static ConstructorHelpers::FObjectFinder<USoundBase> ObjetoMusica(TEXT("SoundWave'/Game/Assetssss/A14/Music_Nivel6.Music_Nivel6'"));
	if (ObjetoMusica.Succeeded())
	{
		MusicaNivel = ObjetoMusica.Object;
	}
}

void AEstadoNivel6::IniciarNivel(AGestorNiveles* Gestor)
{
	if (Gestor && Gestor->AudioComponentCentral && MusicaNivel)
	{
		Gestor->AudioComponentCentral->Stop();
		Gestor->AudioComponentCentral->SetSound(MusicaNivel);
		Gestor->AudioComponentCentral->Play();
	}
	UE_LOG(LogTemp, Warning, TEXT("[Nivel 6] Estado Activado: Cargando la musica del Nivel 6."));
}

void AEstadoNivel6::EjecutarTick(float DeltaTime) {}

