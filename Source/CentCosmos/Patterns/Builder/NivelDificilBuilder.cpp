#include "NivelDificilBuilder.h"

ANivelDificilBuilder::ANivelDificilBuilder()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANivelDificilBuilder::Reset()
{
	NivelActual = FNivel();
	EnemigosGenerados.Empty();
}

void ANivelDificilBuilder::SetMetadatos(const FString& Nombre, float TiempoLimite)
{
	NivelActual.Nombre = Nombre;
	NivelActual.TiempoLimite = TiempoLimite;
}

void ANivelDificilBuilder::SetDificultad(float Dificultad)
{
	NivelActual.Dificultad = Dificultad;
}

void ANivelDificilBuilder::AgregarEnemigos(UWorld* World)
{
	// Tu lógica de enemigos aquí
}

void ANivelDificilBuilder::AgregarAmbientacion(UWorld* World)
{
	// Tu lógica de ambientación aquí
}

void ANivelDificilBuilder::AgregarBoss(UWorld* World)
{
	// Tu lógica del jefe aquí
}

FNivel ANivelDificilBuilder::ObtenerNivel()
{
	NivelActual.Enemigos = EnemigosGenerados;
	return NivelActual;
}