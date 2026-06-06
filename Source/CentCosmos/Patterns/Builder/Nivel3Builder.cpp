#include "Nivel3Builder.h"

ANivel3Builder::ANivel3Builder()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANivel3Builder::Reset()
{
	NivelActual = FNivel();
	EnemigosGenerados.Empty();
}

void ANivel3Builder::SetMetadatos(const FString& Nombre, float TiempoLimite)
{
	NivelActual.Nombre = Nombre;
	NivelActual.TiempoLimite = TiempoLimite;
}

void ANivel3Builder::SetDificultad(float Dificultad)
{
	NivelActual.Dificultad = Dificultad;
}

void ANivel3Builder::AgregarEnemigos(UWorld* World)
{
	// Tu lógica de enemigos aquí
}

void ANivel3Builder::AgregarAmbientacion(UWorld* World)
{
	// Tu lógica de ambientación aquí
}

void ANivel3Builder::AgregarBoss(UWorld* World)
{
	// Tu lógica del jefe aquí
}

FNivel ANivel3Builder::ObtenerNivel()
{
	NivelActual.Enemigos = EnemigosGenerados;
	return NivelActual;
}
