// Fill out your copyright notice in the Description page of Project Settings.
#include "LV06_GameMode.h"

ALV06_GameMode::ALV06_GameMode()
{
    DefaultPawnClass = ACentCosmosPawn::StaticClass();
}

void ALV06_GameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    Builder = Mundo->SpawnActor<ANivel6Builder>();
    Director = Mundo->SpawnActor<ANivelDirector>();

    Director->SetBuilder(Builder);
    Director->ConstruirNivel(Mundo, TEXT("Nivel 6"), 120.0f, 6.0f);
}