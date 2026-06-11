// Fill out your copyright notice in the Description page of Project Settings.


#include "EstadoNivel1.h"
#include "GestorNiveles.h"
#include "UObject/ConstructorHelpers.h"

AEstadoNivel1::AEstadoNivel1()
{
	static ConstructorHelpers::FObjectFinder<USoundBase> ObjetoMusica(TEXT("SoundWave'/Game/Assetssss/A14/Level_1.Level_1'"));
	if (ObjetoMusica.Succeeded())
	{
		MusicaNivel = ObjetoMusica.Object;
	}
}

void AEstadoNivel1::IniciarNivel(AGestorNiveles* Gestor)
{
	if (Gestor && Gestor->AudioComponentCentral && MusicaNivel)
	{
		Gestor->AudioComponentCentral->Stop();
		Gestor->AudioComponentCentral->SetSound(MusicaNivel);
		Gestor->AudioComponentCentral->Play();
	}

	UE_LOG(LogTemp, Warning, TEXT("[Nivel 1] Estado Activado: Cargando la musica del Nivel 1."));
}

void AEstadoNivel1::EjecutarTick(float DeltaTime) {}
