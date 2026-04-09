#include "InventoryWidget.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bAutomaticallyRegisterInputOnConstruction = true;
}

void UInventoryWidget::NativeOnInitialized()
{
	bAutomaticallyRegisterInputOnConstruction = true;
	Super::NativeOnInitialized();
}
