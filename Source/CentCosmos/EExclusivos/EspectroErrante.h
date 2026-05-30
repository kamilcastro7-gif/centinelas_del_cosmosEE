// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EExclusivo.h" // Cabecera de tu clase padre para enemigos exclusivos
#include "EspectroErrante.generated.h"

UCLASS()
class CENTCOSMOS_API AEspectroErrante : public AEExclusivo
{
	GENERATED_BODY()

public:
	// Constructor de la clase
	AEspectroErrante();

protected:
	// Llamado cuando el juego comienza o el actor spawnea
	virtual void BeginPlay() override;

public:
	// Llamado en cada frame
	virtual void Tick(float DeltaTime) override;

	// Flag p�blico para controlar la vulnerabilidad desde los proyectiles del jugador
	UPROPERTY(BlueprintReadOnly, Category = "Espectro")
	bool bEsInvulnerable;

private:
	// Componente de la malla del cuerpo (Shape_Trim)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MallaCuerpo;

	// Componente de la malla del n�cleo central (Sphere)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MallaNucleo;

	// Velocidad a la que se desplaza el espectro
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion", meta = (AllowPrivateAccess = "true"))
	float VelocidadFlotacion = 320.0f; // �Subida brutal de velocidad para que vuele por el mapa!

	// Handles para el control de los tiempos de fase
	FTimerHandle TimerFaseInvisibleHandle;
	FTimerHandle TimerFaseExpuestoHandle;

	// Handle para el bucle del rumbo aleatorio
	FTimerHandle TimerRumboHandle;

	// Vector que almacena la direcci�n del movimiento errante
	FVector DireccionErrante;

	// Macro UFUNCTION obligatoria para que Unreal pueda enlazar el Timer correctamente
	UFUNCTION()
	void CambiarDireccionAleatoria();

	// Funciones de control de estado de la IA
	void EntrarFaseInvisible();
	void EntrarFaseExpuesto();
	void EjecutarDisparoEspectral();
};