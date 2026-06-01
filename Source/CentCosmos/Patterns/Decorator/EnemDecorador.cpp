#include "EnemDecorador.h"
#include "Player/CentCosmosPawn.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UEnemDecorador::AplicarSobrecargaApex(ACentCosmosPawn* Nave)
{
    if (!Nave) return;

    Nave->bTieneSobreCargaApex = true;
    Nave->MoveSpeed  = Nave->MoveSpeedBase * 1.5f;
    Nave->FireRate   = Nave->FireRateBase  * 0.5f;

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
            TEXT("SobreCarga Apex activada!"));

    FTimerHandle Handle;
    Nave->GetWorldTimerManager().SetTimer(Handle,
        FTimerDelegate::CreateLambda([Nave]()
        {
            if (!IsValid(Nave)) return;
            Nave->DesactivarSobreCargaApex();
        }),
        DuracionSobrecargaApex, false);
}

void UEnemDecorador::AplicarDisparoTriple(ACentCosmosPawn* Nave)
{
    if (!Nave) return;

    Nave->bTieneDisparoTriple = true;

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
            TEXT("Disparo Triple activado!"));

    FTimerHandle Handle;
    Nave->GetWorldTimerManager().SetTimer(Handle,
        FTimerDelegate::CreateLambda([Nave]()
        {
            if (!IsValid(Nave)) return;
            Nave->DesactivarDisparoTriple();
        }),
        DuracionDisparoTriple, false);
}