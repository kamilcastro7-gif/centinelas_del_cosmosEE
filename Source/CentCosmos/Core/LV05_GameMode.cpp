// Fill out your copyright notice in the Description page of Project Settings.
#include "LV05_GameMode.h"

ALV05_GameMode::ALV05_GameMode()
{
    DefaultPawnClass = ACentCosmosPawn::StaticClass();
}

void ALV05_GameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    Builder = Mundo->SpawnActor<ANivel5Builder>();
    Director = Mundo->SpawnActor<ANivelDirector>();

    Director->SetBuilder(Builder);
    Director->ConstruirNivel(Mundo, TEXT("Nivel 5"), 120.0f, 5.0f);
}