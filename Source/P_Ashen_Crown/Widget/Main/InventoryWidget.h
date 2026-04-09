#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/Widget/MovableWindowWidgetBase.h"
#include "InventoryWidget.generated.h"

/**
 * 인벤토리 위젯
 */
UCLASS()
class P_ASHEN_CROWN_API UInventoryWidget : public UMovableWindowWidgetBase
{
	GENERATED_BODY()
	
public:
	UInventoryWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;
};
