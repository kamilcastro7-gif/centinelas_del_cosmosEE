#include "NivelMedioBuilder.h"
#include "Patterns/Decorator/EnemBasico.h"
#include "Patterns/Decorator/EnemTanque.h"
#include "Patterns/Decorator/EnemVida.h"

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

void ANivelMedioBuilder::AgregarEnemigos(UWorld* World, const TArray<AActor*>& Plantillas)
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
        else if (Plantilla->IsA(AEnemTanque::StaticClass()))
        {
            AEnemTanque* Tanque = World->SpawnActor<AEnemTanque>(AEnemTanque::StaticClass(), Plantilla->GetActorLocation(), Plantilla->GetActorRotation(), Params);
            if (Tanque)
            {
                UEnemVida* Decorado = NewObject<UEnemVida>();
                Decorado->Initialize(Tanque, 150.0f);
                Tanque->SetVida(Decorado->GetVida());
                EnemigosGenerados.Add(Tanque);
            }
        }
    }
}

void ANivelMedioBuilder::AgregarBoss(UWorld* World, AActor* Boss)
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

FNivel ANivelMedioBuilder::ObtenerNivel()
{
    NivelActual.Enemigos = EnemigosGenerados;
    return NivelActual;
}