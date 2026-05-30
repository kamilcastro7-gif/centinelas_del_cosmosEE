#include "GestionStamina.h"

UGestionStamina::UGestionStamina()
{
    StaminaActual = 100.0f;
    StaminaMaxima = 100.0f;
    RegeneracionPorSegundo = 15.0f;
    Inner = nullptr;
}

void UGestionStamina::Aplicar()
{
    if (UGestionStamina* Base = Cast<UGestionStamina>(Inner))
        StaminaActual = FMath::Min(StaminaActual + Base->GetStaminaActual(), StaminaMaxima);
}

void UGestionStamina::Inicializar() { StaminaActual = StaminaMaxima; }

void UGestionStamina::Regenerar(float DeltaTime)
{
    if (StaminaActual < StaminaMaxima)
        StaminaActual = FMath::Min(StaminaActual + RegeneracionPorSegundo * DeltaTime, StaminaMaxima);
}

void UGestionStamina::Restaurar() { StaminaActual = StaminaMaxima; }

bool UGestionStamina::Consumir(float Costo)
{
    if (StaminaActual >= Costo) { StaminaActual -= Costo; return true; }
    return false;
}
