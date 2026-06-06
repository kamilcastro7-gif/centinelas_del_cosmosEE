#include "Nivel2Builder.h"

ANivel2Builder::ANivel2Builder()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANivel2Builder::Reset()
{
	NivelActual = FNivel();
	EnemigosGenerados.Empty();
}

void ANivel2Builder::SetMetadatos(const FString& Nombre, float TiempoLimite)
{
	NivelActual.Nombre = Nombre;
	NivelActual.TiempoLimite = TiempoLimite;
}

void ANivel2Builder::SetDificultad(float Dificultad)
{
	NivelActual.Dificultad = Dificultad;
}

void ANivel2Builder::AgregarEnemigos(UWorld* World)
{
	// Tu lógica de enemigos aquí
}

void ANivel2Builder::AgregarAmbientacion(UWorld* World)
{
	// Tu lógica de ambientación aquí
}

void ANivel2Builder::AgregarBoss(UWorld* World)
{
	// Tu lógica del jefe aquí
}

FNivel ANivel2Builder::ObtenerNivel()
{
	NivelActual.Enemigos = EnemigosGenerados;
	return NivelActual;
}
