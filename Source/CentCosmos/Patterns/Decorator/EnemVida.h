	#pragma once

	#include "EnemDecorador.h"
	#include "EnemVida.generated.h"

	UCLASS(BlueprintType)
	class CENTCOSMOS_API UEnemVida : public UEnemDecorador
	{
		GENERATED_BODY()

	private:
		float VidaBonus;

	public:
		void Initialize(UObject* InInner, float InVidaBonus)
		{
			Super::Initialize(InInner);
			VidaBonus = InVidaBonus;
		}

		virtual float GetVida() const override
		{
			return (Inner ? Inner->GetVida() : 0.0f) + VidaBonus;
		}
	};