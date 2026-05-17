// Fill out your copyright notice in the Description page of Project Settings.

#include "Facade.h"
#include "EVastago_Del_infierno.h"
#include "EVigia_Del_Vacio.h"
#include "EHeraldo_De_La_Ruina.h"
#include "Kismet/GameplayStatics.h"

AFacade::AFacade() { PrimaryActorTick.bCanEverTick = false; }

void AFacade::MoverHorda() {
    TArray<AActor*> Encontrados;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Enemigo"), Encontrados);

    for (AActor* Actor : Encontrados) {
        if (auto* Vas = Cast<AEVastago_Del_infierno>(Actor)) Vas->moverVastago();
        else if (auto* Vig = Cast<AEVigia_Del_Vacio>(Actor)) Vig->moverVigia();
        else if (auto* Her = Cast<AEHeraldo_De_La_Ruina>(Actor)) Her->moverHeraldo();
    }
}

