#include "MovableWindowWidgetBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"

UMovableWindowWidgetBase::UMovableWindowWidgetBase(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer),
	bIsDragging(false),
	mPreMousePos(),
	mWindowButton(nullptr)
{
}

void UMovableWindowWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	mWindowButton = Cast<UButton>(GetWidgetFromName(TEXT("WindowButton")));
	if (mWindowButton != nullptr)
	{
		mWindowButton->OnPressed.AddDynamic(this, &UMovableWindowWidgetBase::BeginDragWindow);
		mWindowButton->OnReleased.AddDynamic(this, &UMovableWindowWidgetBase::EndDragWindow);
	}
}

void UMovableWindowWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsDragging == true)
	{
		FVector2D CurMousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		FVector2D MouseOffset = CurMousePos - mPreMousePos;

		if (IsValid(mDragSlot) == true)
		{
			mDragSlot->SetPosition(mDragSlot->GetPosition() + MouseOffset);
		}
		mPreMousePos = CurMousePos;
	}
}

void UMovableWindowWidgetBase::BeginDragWindow()
{
	for (UWidget* CurWidget = mWindowButton; CurWidget != nullptr; CurWidget = CurWidget->GetParent())
	{
		mDragSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CurWidget);
		if (mDragSlot != nullptr)
		{
			break;
		}
	}
	if (IsValid(mDragSlot) == true)
	{
		bIsDragging = true;
		mPreMousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		mDragSlot->Parent->ShiftChild(mDragSlot->Parent->GetChildrenCount() - 1, mDragSlot->GetContent());
	}
}

void UMovableWindowWidgetBase::EndDragWindow()
{
	if (bIsDragging == true)
	{
		bIsDragging = false;
		mDragSlot = nullptr;
		mPreMousePos = FVector2D::Zero();

		PlaySound(mDropSound);
	}
}

bool UMovableWindowWidgetBase::IsDragging() const
{
	return bIsDragging;
}
