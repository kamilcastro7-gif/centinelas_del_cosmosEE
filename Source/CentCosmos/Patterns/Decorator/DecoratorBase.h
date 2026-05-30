// DecoratorBase.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DecoratorBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class CENTCOSMOS_API UDecoratorBase : public UObject
{
    GENERATED_BODY()

public:
    UDecoratorBase();

    UFUNCTION(BlueprintCallable, Category = "Decorator")
    void SetInner(UObject* InnerComponent);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Decorator")
    UObject* GetInner() const;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Decorator")
    UObject* Inner;
};
