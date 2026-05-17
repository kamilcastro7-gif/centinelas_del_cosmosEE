// Fill out your copyright notice in the Description page of Project Settings.

#include "Enjambre.h"
#include "Components/StaticMeshComponent.h"

AEnjambre::AEnjambre()
{
    PrimaryActorTick.bCanEverTick = true;
    MallaEnemigo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaEnemigo"));
    RootComponent = MallaEnemigo;
}

void AEnjambre::BeginPlay() { Super::BeginPlay(); }
void AEnjambre::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

