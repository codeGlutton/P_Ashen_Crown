#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "Blueprint/UserWidget.h"
#include "DragDropWidgetBase.generated.h"

/************************
   IDragDropViewHandler
*************************/

UINTERFACE(MinimalAPI)
class UDragDropViewHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 드래그 드롭이 가능한 View Handler 인터페이스
 */
class P_ASHEN_CROWN_API IDragDropViewHandler
{
	GENERATED_BODY()

public:
	/**
	 * 드래그 시작 시 호출되는 함수
	 */
	virtual void OnBeginDrag() = 0;
	/**
	 * 드래그 종료 시 호출되는 함수
	 */
	virtual void OnEndDrag() = 0;
	/**
	 * 드랍 시 호출되는 함수
	 * @param DragHandler 드랍되는 대상 handler
	 */
	virtual void OnDrop(IDragDropViewHandler* DragHandler) = 0;
};

/************************
	 UDropWidgetBase
*************************/

/**
 * 드롭 가능한 위젯 베이스
 */
UCLASS(Abstract)
class P_ASHEN_CROWN_API UDropWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UDropWidgetBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
	// 드래그될 뷰 모델 이름
	UPROPERTY(EditDefaultsOnly, Category = "DropWidgetBase")
	FName mDragViewModelName;
	// 드랍될 뷰 모델 이름
	UPROPERTY(EditDefaultsOnly, Category = "DropWidgetBase")
	FName mDropViewModelName;

	UPROPERTY(EditDefaultsOnly, Category = "DropWidgetBase", meta = (DisplayName = "DropSound"))
	TObjectPtr<USoundBase> mDropSound;
};


/************************
	UDragDropWidgetBase
*************************/

/**
 * 드래그 후 드롭으로 이동 가능한 위젯 베이스
 */
UCLASS(Abstract)
class P_ASHEN_CROWN_API UDragDropWidgetBase : public UDropWidgetBase
{
	GENERATED_BODY()
	
public:
	UDragDropWidgetBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	UFUNCTION(BlueprintCallable, Category = "DragDropWidgetBase")
	void SetCanDrag(bool bWillCanDrag);
	UFUNCTION(BlueprintCallable, Category = "DragDropWidgetBase")
	bool CanDrag() const;

protected:
	// 드래그 시 보여줄 위젯
	UPROPERTY(EditDefaultsOnly, Category = "DragDropWidgetBase")
	TSubclassOf<UUserWidget> mDragWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "DropWidgetBase", meta = (DisplayName = "DragDetectSound"))
	TObjectPtr<USoundBase> mDragDetectSound;

public:
	UPROPERTY()
	FGetBool bCanDragDelegate;

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetCanDrag", Getter = "CanDrag", BlueprintSetter = "SetCanDrag", BlueprintGetter="CanDrag", meta = (AllowPrivateAccess))
	bool bCanDrag;
};
