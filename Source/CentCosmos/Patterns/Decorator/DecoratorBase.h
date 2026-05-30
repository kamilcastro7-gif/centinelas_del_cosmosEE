#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DecoratorBase.generated.h"

UCLASS(Abstract)
class CENTCOSMOS_API UDecoratorBase : public UObject
{
    GENERATED_BODY()

public:
    UDecoratorBase();

    virtual void Aplicar() {}

    UObject* GetInner() const;
    void SetInner(UObject* InnerComponent);

protected:
    UPROPERTY()
    UObject* Inner;
};
