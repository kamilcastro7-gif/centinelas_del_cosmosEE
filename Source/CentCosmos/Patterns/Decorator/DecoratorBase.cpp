#include "DecoratorBase.h"

UDecoratorBase::UDecoratorBase()
    : Inner(nullptr)
{
}

void UDecoratorBase::SetInner(UObject* InnerComponent)
{
    Inner = InnerComponent;
}

UObject* UDecoratorBase::GetInner() const
{
    return Inner;
}
