#include "NivelMedioBuilder.h"

ANivelMedioBuilder::ANivelMedioBuilder()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANivelMedioBuilder::Reset()
{
	NivelActual = FNivel();
	EnemigosGenerados.Empty();
}

void ANivelMedioBuilder::SetMetadatos(const FString& Nombre, float TiempoLimite)
{
	NivelActual.Nombre = Nombre;
	NivelActual.TiempoLimite = TiempoLimite;
}

void ANivelMedioBuilder::SetDificultad(float Dificultad)
{
	NivelActual.Dificultad = Dificultad;
}

void ANivelMedioBuilder::AgregarEnemigos(UWorld* World)
{
	// Tu lógica de enemigos aquí
}

void ANivelMedioBuilder::AgregarAmbientacion(UWorld* World)
{
	// Tu lógica de ambientación aquí
}

void ANivelMedioBuilder::AgregarBoss(UWorld* World)
{
	// Tu lógica del jefe aquí
}

FNivel ANivelMedioBuilder::ObtenerNivel()
{
	NivelActual.Enemigos = EnemigosGenerados;
	return NivelActual;
}