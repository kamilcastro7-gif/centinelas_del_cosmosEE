#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INivelBuilder.h"
#include "EnemyFactory.h"
#include "GeneradorAmbientacion.h"          
#include "Nivel1Builder.generated.h"

UCLASS()
class CENTCOSMOS_API ANivel1Builder : public AActor, public INivelBuilder
{
    GENERATED_BODY()
public:
    ANivel1Builder();
    virtual void Reset() override;
    virtual void SetMetadatos(const FString& Nombre, float TiempoLimite) override;
    virtual void SetDificultad(float Dificultad) override;
    virtual void AgregarAmbientacion(UWorld* World) override;
    virtual void AgregarEnemigos(UWorld* World) override;
    virtual FNivel ObtenerNivel() override;
    virtual void Tick(float DeltaTime) override;

private:
    FNivel NivelActual;
    TArray<TWeakObjectPtr<AActor>> EnemigosGenerados;
    TArray<TWeakObjectPtr<AActor>> EnemigosOla1;
    bool bBossSpawneado;
    void SpawnVastagos(AEnemyFactory* Factory, int32 Cantidad);
    void SpawnVigias(AEnemyFactory* Factory, int32 Cantidad);
    void SpawnHeraldos(AEnemyFactory* Factory, int32 Cantidad);
    void SpawnDronAnclaje(AEnemyFactory* Factory, int32 Cantidad);
    void SpawnNaves(AGeneradorAmbientacion* Gen, int32 Cantidad); 
    void SpawnSatelites(AGeneradorAmbientacion* Gen, int32 Cantidad);
    void SpawnRestos(AGeneradorAmbientacion* Gen, int32 Cantidad);
    void VerificarYSpawnearBoss(UWorld* World);
};