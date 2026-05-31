#include "NivelDirector.h"
#include "INivelBuilder.h"

ANivelDirector::ANivelDirector()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANivelDirector::SetBuilder(TScriptInterface<INivelBuilder> NuevoBuilder)
{
    Builder = NuevoBuilder;
}

FNivel ANivelDirector::ConstruirNivelCompleto(UWorld* World, const FString& Nombre, float TiempoLimite, float Dificultad, const TArray<AActor*>& Plantillas, AActor* Boss)
{
    if (!Builder || !World) return FNivel();

    Builder->Reset();
    Builder->SetMetadatos(Nombre, TiempoLimite);
    Builder->SetDificultad(Dificultad);
    Builder->AgregarEnemigos(World, Plantillas);
    if (Boss)
    {
        TArray<AActor*> BossWrapper = { Boss };
        Builder->AgregarBoss(World, Boss);
    }
    return Builder->ObtenerNivel();
}

FNivel ANivelDirector::ConstruirTutorial(UWorld* World, const TArray<AActor*>& Plantillas)
{
    if (!Builder || !World) return FNivel();

    Builder->Reset();
    Builder->SetMetadatos("Tutorial", 999.0f);
    Builder->SetDificultad(0.0f);
    Builder->AgregarEnemigos(World, Plantillas);
    return Builder->ObtenerNivel();
}

FNivel ANivelDirector::ConstruirOleada(UWorld* World, const TArray<AActor*>& Plantillas)
{
    if (!Builder || !World) return FNivel();

    Builder->Reset();
    Builder->SetMetadatos("Oleada", 60.0f);
    Builder->SetDificultad(5.0f);
    Builder->AgregarEnemigos(World, Plantillas);
    return Builder->ObtenerNivel();
}