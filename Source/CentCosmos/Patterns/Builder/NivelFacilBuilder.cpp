#include "NivelFacilBuilder.h"
#include "Patterns/Decorator/EnemBasico.h"
#include "Patterns/Decorator/EnemRapido.h"

ANivelFacilBuilder::ANivelFacilBuilder()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANivelFacilBuilder::Reset()
{
    NivelActual = FNivel();
    EnemigosGenerados.Empty();
}

void ANivelFacilBuilder::SetMetadatos(const FString& Nombre, float TiempoLimite)
{
    NivelActual.Nombre = Nombre;
    NivelActual.TiempoLimite = TiempoLimite;
}

void ANivelFacilBuilder::SetDificultad(float Dificultad)
{
    NivelActual.Dificultad = Dificultad;
}

void ANivelFacilBuilder::AgregarEnemigos(UWorld* World, const TArray<AActor*>& Plantillas)
{
    if (!World) return;

    for (AActor* Plantilla : Plantillas)
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        if (Plantilla->IsA(AEnemBasico::StaticClass()))
        {
            AEnemBasico* Basico = World->SpawnActor<AEnemBasico>(AEnemBasico::StaticClass(), Plantilla->GetActorLocation(), Plantilla->GetActorRotation(), Params);
            if (Basico) EnemigosGenerados.Add(Basico);
        }
        else if (Plantilla->IsA(AEnemRapido::StaticClass()))
        {
            AEnemRapido* Rapido = World->SpawnActor<AEnemRapido>(AEnemRapido::StaticClass(), Plantilla->GetActorLocation(), Plantilla->GetActorRotation(), Params);
            if (Rapido) EnemigosGenerados.Add(Rapido);
        }
    }
}

void ANivelFacilBuilder::AgregarBoss(UWorld* World, AActor* Boss)
{
    if (!World || !Boss) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* Spawned = World->SpawnActor<AActor>(Boss->GetClass(), Boss->GetActorLocation(), Boss->GetActorRotation(), Params);
    if (Spawned)
    {
        EnemigosGenerados.Add(Spawned);
    }
}

FNivel ANivelFacilBuilder::ObtenerNivel()
{
    NivelActual.Enemigos = EnemigosGenerados;
    return NivelActual;
}