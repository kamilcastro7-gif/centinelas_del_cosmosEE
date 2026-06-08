// Fill out your copyright notice in the Description page of Project Settings.
#include "LV04_GameMode.h"

ALV04_GameMode::ALV04_GameMode()
{
    DefaultPawnClass = ACentCosmosPawn::StaticClass();
}

void ALV04_GameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    Builder = Mundo->SpawnActor<ANivel4Builder>();
    Director = Mundo->SpawnActor<ANivelDirector>();

    Director->SetBuilder(Builder);
    Director->ConstruirNivel(Mundo, TEXT("Nivel 4"), 120.0f, 4.0f);
}