#include "Nivel4Builder.h"
#include "EnemyFactory.h"
#include "GeneradorAmbientacion.h"
#include "Kismet/GameplayStatics.h"
#include "ArenaCombate.h"
#include "Tribunal_Ira.h"
#include "Engine/Engine.h" // Para los mensajes en pantalla
// IMPORTANTE: Incluye aquí las clases de tus obstáculos para poder destruirlos
#include "ObstaculoNave.h"
#include "ObstaculoSatelite.h"
#include "ObstaculoRestos.h"

ANivel4Builder::ANivel4Builder()
{
    PrimaryActorTick.bCanEverTick = true;
    bBossSpawneado = false;
    Factory = nullptr;
    Generador = nullptr;
}

void ANivel4Builder::BeginPlay()
{
    Super::BeginPlay();

    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    // Buscamos o creamos el Factory
    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(Mundo, AEnemyFactory::StaticClass(), Found);
    if (Found.Num() > 0) Factory = Cast<AEnemyFactory>(Found[0]);
    else Factory = Mundo->SpawnActor<AEnemyFactory>(AEnemyFactory::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

    // Buscamos o creamos el Generador
    Found.Empty();
    UGameplayStatics::GetAllActorsOfClass(Mundo, AGeneradorAmbientacion::StaticClass(), Found);
    if (Found.Num() > 0) Generador = Cast<AGeneradorAmbientacion>(Found[0]);
    else Generador = Mundo->SpawnActor<AGeneradorAmbientacion>(AGeneradorAmbientacion::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
}

void ANivel4Builder::Reset()
{
    EnemigosOla1.Empty();
    bBossSpawneado = false;
}

void ANivel4Builder::SetMetadatos(const FString& Nombre, float Duracion) {}

void ANivel4Builder::SetDificultad(float Nivel) {}

void ANivel4Builder::AgregarAmbientacion(UWorld* Mundo)
{
    // AQUI YA NO SPAWNEAMOS LA ARENA, SOLO LA AMBIENTACION ESPACIAL
    if (!Generador) return;

    SpawnNaves(Generador, 8);
    SpawnSatelites(Generador, 8);
    SpawnRestos(Generador, 8);
    SpawnRastrosFuego(Generador, 6);
}

void ANivel4Builder::AgregarEnemigos(UWorld* Mundo)
{
    if (!Factory) return;

    SpawnVastagos(Factory, 16);
    SpawnVigias(Factory, 4);
    SpawnHeraldos(Factory, 3);
    SpawnBombarderosFuego(Factory, 2);
}

FNivel ANivel4Builder::ObtenerNivel()
{
    return FNivel();
}

void ANivel4Builder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Lógica exacta del Nivel 1: Si el array está vacío (aún no spawnean) o el boss ya salió, no hacemos nada.
    if (bBossSpawneado || EnemigosOla1.Num() == 0) return;

    VerificarYSpawnearBoss();
}

void ANivel4Builder::SpawnVastagos(AEnemyFactory* F, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector  Pos = FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.f);
        AActor* E = F->FabricarVastago(Pos, FRotator::ZeroRotator);
        if (E) EnemigosOla1.Add(E);
    }
}

void ANivel4Builder::SpawnVigias(AEnemyFactory* F, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector  Pos = FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.f);
        AActor* E = F->FabricarVigia(Pos, FRotator::ZeroRotator);
        if (E) EnemigosOla1.Add(E);
    }
}

void ANivel4Builder::SpawnHeraldos(AEnemyFactory* F, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector  Pos = FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.f);
        AActor* E = F->FabricarHeraldo(Pos, FRotator::ZeroRotator);
        if (E) EnemigosOla1.Add(E);
    }
}

void ANivel4Builder::SpawnBombarderosFuego(AEnemyFactory* F, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector  Pos = FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.f);
        AActor* E = F->FabricarBombarderoFuego(Pos, FRotator::ZeroRotator);
        if (E) EnemigosOla1.Add(E);
    }
}

void ANivel4Builder::SpawnNaves(AGeneradorAmbientacion* G, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        G->FabricarNave(Pos, FRotator::ZeroRotator);
    }
}

void ANivel4Builder::SpawnSatelites(AGeneradorAmbientacion* G, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        G->FabricarSatelite(Pos, FRotator::ZeroRotator);
    }
}

void ANivel4Builder::SpawnRestos(AGeneradorAmbientacion* G, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        G->FabricarRestos(Pos, FRotator::ZeroRotator);
    }
}

void ANivel4Builder::SpawnRastrosFuego(AGeneradorAmbientacion* G, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        G->FabricarRastrosFuego(Pos, FRotator::ZeroRotator);
    }
}

void ANivel4Builder::VerificarYSpawnearBoss()
{
    // Lógica exacta del Nivel 1: Contar cuántos punteros siguen siendo válidos
    int32 Vivos = 0;
    for (const TWeakObjectPtr<AActor>& Ref : EnemigosOla1)
    {
        if (Ref.IsValid()) Vivos++;
    }

    // Si todavía quedan enemigos vivos, salimos y seguimos esperando en el Tick
    if (Vivos > 0) return;

    // Si llegamos aquí, ¡todos murieron!
    bBossSpawneado = true;

    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    TArray<AActor*> Encontrados;

    // 1. Destruir la ambientación inicial para limpiar la pantalla
    UGameplayStatics::GetAllActorsOfClass(Mundo, AObstaculoNave::StaticClass(), Encontrados);
    for (AActor* A : Encontrados) A->Destroy();

    UGameplayStatics::GetAllActorsOfClass(Mundo, AObstaculoSatelite::StaticClass(), Encontrados);
    for (AActor* A : Encontrados) A->Destroy();

    UGameplayStatics::GetAllActorsOfClass(Mundo, AObstaculoRestos::StaticClass(), Encontrados);
    for (AActor* A : Encontrados) A->Destroy();

    // NOTA: Si creaste una clase específica para "RastrosFuego", añádela aquí también para destruirla.

    UGameplayStatics::GetAllActorsOfClass(Mundo, AGeneradorAmbientacion::StaticClass(), Encontrados);
    for (AActor* A : Encontrados) A->Destroy();

    // 2. Aparece la Arena de Combate
    Mundo->SpawnActor<AArenaCombate>(AArenaCombate::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    // 3. Aparece el Jefe (Tribunal de Ira)
    FVector PosBoss = FVector(0.f, 600.f, 0.f);
    Mundo->SpawnActor<ATribunal_Ira>(ATribunal_Ira::StaticClass(), PosBoss, FRotator::ZeroRotator, Params);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("[Nivel 4] OLA 1 COMPLETADA — ¡Arena y Tribunal de Ira aparecen!"));
}