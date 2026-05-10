// Fill out your copyright notice in the Description page of Project Settings.


#include "Enjambre.h"


AEnjambre::AEnjambre()
{
	PrimaryActorTick.bCanEverTick = true; // Si vas a usar el Facade, podrías poner esto en false luego

	MallaEnemigo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaEnemigo"));
	RootComponent = MallaEnemigo;
}


void AEnjambre::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnjambre::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

