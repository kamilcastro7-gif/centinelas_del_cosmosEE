#include "NivelDificilBuilder.h"
#include "Patterns/Decorator/EnemBasico.h"
#include "Patterns/Decorator/EnemRapido.h"
#include "Patterns/Decorator/EnemTanque.h"
#include "Patterns/Decorator/EnemBoss.h"
#include "Patterns/Decorator/EnemVida.h"

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

void ANivelDificilBuilder::AgregarEnemigos(UWorld* World, const TArray<AActor*>& Plantillas)
{
    if (!World) return;

    for (AActor* Plantilla : Plantillas)
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        if (Plantilla->IsA(AEnemBasico::StaticClass()))
        {
            AEnemBasico* Basico = World->SpawnActor<AEnemBasico>(AEnemBasico::StaticClass(), Plantilla->GetActorLocation(), Plantilla->GetActorRotation(), Params);
            if (Basico)
            {
                UEnemVida* Decorado = NewObject<UEnemVida>();
                Decorado->Initialize(Basico, 50.0f);
                Basico->SetVida(Decorado->GetVida());
                EnemigosGenerados.Add(Basico);
            }
        }
        else if (Plantilla->IsA(AEnemRapido::StaticClass()))
        {
            AEnemRapido* Rapido = World->SpawnActor<AEnemRapido>(AEnemRapido::StaticClass(), Plantilla->GetActorLocation(), Plantilla->GetActorRotation(), Params);
            if (Rapido)
            {
                UEnemVida* Decorado = NewObject<UEnemVida>();
                Decorado->Initialize(Rapido, 30.0f);
                Rapido->SetVida(Decorado->GetVida());
                EnemigosGenerados.Add(Rapido);
            }
        }
        else if (Plantilla->IsA(AEnemTanque::StaticClass()))
        {
            AEnemTanque* Tanque = World->SpawnActor<AEnemTanque>(AEnemTanque::StaticClass(), Plantilla->GetActorLocation(), Plantilla->GetActorRotation(), Params);
            if (Tanque)
            {
                UEnemVida* Decorado1 = NewObject<UEnemVida>();
                Decorado1->Initialize(Tanque, 150.0f);
                UEnemVida* Decorado2 = NewObject<UEnemVida>();
                Decorado2->Initialize(Decorado1, 100.0f);
                Tanque->SetVida(Decorado2->GetVida());
                EnemigosGenerados.Add(Tanque);
            }
        }
    }
}

void ANivelDificilBuilder::AgregarBoss(UWorld* World, AActor* Boss)
{
    if (!World || !Boss) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* Spawned = World->SpawnActor<AActor>(Boss->GetClass(), Boss->GetActorLocation(), Boss->GetActorRotation(), Params);
    if (Spawned)
    {
        AEnemBoss* BossEnem = Cast<AEnemBoss>(Spawned);
        if (BossEnem)
        {
            UEnemVida* Decorado = NewObject<UEnemVida>();
            Decorado->Initialize(BossEnem, 500.0f);
            BossEnem->SetVida(Decorado->GetVida());
            EnemigosGenerados.Add(BossEnem);
        }
        else
        {
            EnemigosGenerados.Add(Spawned);
        }
    }
}

FNivel ANivelDificilBuilder::ObtenerNivel()
{
    NivelActual.Enemigos = EnemigosGenerados;
    return NivelActual;
}