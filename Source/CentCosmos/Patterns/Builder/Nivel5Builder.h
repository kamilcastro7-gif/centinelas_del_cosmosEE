#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INivelBuilder.h"
#include "Nivel5Builder.generated.h"

class AEnemyFactory;
class AGeneradorAmbientacion;

UCLASS()
class CENTCOSMOS_API ANivel5Builder : public AActor, public INivelBuilder
{
    GENERATED_BODY()

public:
    ANivel5Builder();

    virtual void   Reset()                                             override;
    virtual void   SetMetadatos(const FString& Nombre, float Duracion) override;
    virtual void   SetDificultad(float Nivel)                          override;
    virtual void   AgregarAmbientacion(UWorld* Mundo)                  override;
    virtual void   AgregarEnemigos(UWorld* Mundo)                      override;
    virtual FNivel ObtenerNivel()                                      override;
    virtual void   Tick(float DeltaTime)                               override;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY() AEnemyFactory* Factory;
    UPROPERTY() AGeneradorAmbientacion* Generador;

    TArray<TWeakObjectPtr<AActor>> EnemigosOla1;
    bool bBossSpawneado;

    void SpawnVastagos(AEnemyFactory* F, int32 N);
    void SpawnVigias(AEnemyFactory* F, int32 N);
    void SpawnHeraldos(AEnemyFactory* F, int32 N);
    void SpawnEspectrosErrantes(AEnemyFactory* F, int32 N);

    void SpawnNaves(AGeneradorAmbientacion* G, int32 N);
    void SpawnSatelites(AGeneradorAmbientacion* G, int32 N);
    void SpawnRestos(AGeneradorAmbientacion* G, int32 N);
    void SpawnGrietasAntimateria(AGeneradorAmbientacion* G, int32 N);

    void VerificarYSpawnearBoss();
};