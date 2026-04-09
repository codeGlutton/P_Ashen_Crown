#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MovableWindowWidgetBase.generated.h"

class UCanvasPanelSlot;

/**
 * 창을 움직여 배치할 수 있는 위젯 베이스 (부모 위젯이 캔버스 패널이여야 함)
 */
UCLASS(Abstract)
class P_ASHEN_CROWN_API UMovableWindowWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMovableWindowWidgetBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UFUNCTION()
	void BeginDragWindow();
	UFUNCTION()
	void EndDragWindow();

public:
	bool IsDragging() const;

protected:
	bool bIsDragging;
	UCanvasPanelSlot* mDragSlot;
	FVector2D mPreMousePos;

	// 윈도우 버튼
	TObjectPtr<UButton> mWindowButton;

	UPROPERTY(EditDefaultsOnly, Category = "MovableWindowWidgetBase", meta = (DisplayName = "DropSound"))
	TObjectPtr<USoundBase> mDropSound;
};
