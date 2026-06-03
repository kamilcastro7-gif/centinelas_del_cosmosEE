#include "NivelDirector.h"

ANivelDirector::ANivelDirector()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANivelDirector::SetBuilder(TScriptInterface<INivelBuilder> NuevoBuilder)
{
    Builder = NuevoBuilder;
}

FNivel ANivelDirector::ConstruirNivel(UWorld* World, const FString& Nombre, float TiempoLimite, float Dificultad)
{
    if (!Builder || !World) return FNivel();
    Builder->Reset();
    Builder->SetMetadatos(Nombre, TiempoLimite);
    Builder->SetDificultad(Dificultad);
    Builder->AgregarAmbientacion(World);
    Builder->AgregarEnemigos(World);
    return Builder->ObtenerNivel();
}

FNivel ANivelDirector::ConstruirTutorial(UWorld* World)
{
    if (!Builder || !World) return FNivel();
    Builder->Reset();
    Builder->SetMetadatos(TEXT("Tutorial"), 999.0f);
    Builder->SetDificultad(0.0f);
    Builder->AgregarAmbientacion(World);
    Builder->AgregarEnemigos(World);
    return Builder->ObtenerNivel();
}

FNivel ANivelDirector::ConstruirOleada(UWorld* World, float Dificultad)
{
    if (!Builder || !World) return FNivel();
    Builder->Reset();
    Builder->SetMetadatos(TEXT("Oleada"), 60.0f);
    Builder->SetDificultad(Dificultad);
    Builder->AgregarEnemigos(World);
    return Builder->ObtenerNivel();
}