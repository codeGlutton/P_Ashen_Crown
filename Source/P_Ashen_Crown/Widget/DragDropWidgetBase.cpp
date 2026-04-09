#include "DragDropWidgetBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "View/MVVMView.h"
#include "MVVMViewModelBase.h"

/************************
	 UDropWidgetBase
*************************/

UDropWidgetBase::UDropWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDropWidgetBase::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (InOperation != nullptr && InOperation->DefaultDragVisual != nullptr)
	{
		auto* DragUserWidget = Cast<UUserWidget>(InOperation->DefaultDragVisual);
		if (DragUserWidget != nullptr)
		{
			auto* OwnMVVMView = Cast<UMVVMView>(GetExtension(UMVVMView::StaticClass()));
			auto* DragMVVMView = Cast<UMVVMView>(DragUserWidget->GetExtension(UMVVMView::StaticClass()));
			if (IsValid(OwnMVVMView) == true && IsValid(DragMVVMView) == true)
			{
				auto* OwnViewHandler = Cast<IDragDropViewHandler>(OwnMVVMView->GetViewModel(mDropViewModelName).GetObject());
				auto* DragViewHandler = Cast<IDragDropViewHandler>(DragMVVMView->GetViewModel(mDragViewModelName).GetObject());
				if (OwnViewHandler != nullptr && DragViewHandler != nullptr)
				{
					DragViewHandler->OnEndDrag();
					OwnViewHandler->OnDrop(DragViewHandler);
					PlaySound(mDropSound);
					return true;
				}
			}
		}
	}

	return false;
}

/************************
	UDragDropWidgetBase
*************************/

UDragDropWidgetBase::UDragDropWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FReply UDragDropWidgetBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Result;
	Result.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		Result = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}

	return Result.NativeReply;
}

void UDragDropWidgetBase::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (bCanDrag == false)
	{
		return;
	}

	if (OutOperation == nullptr)
	{
		OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
		OutOperation->Pivot = EDragPivot::MouseDown;

		UUserWidget* NewDragUserWidget = CreateWidget(GetOwningPlayer(), mDragWidgetClass);
		if (IsValid(NewDragUserWidget) == false)
		{
			return;
		}

		auto* OwnMVVMView = Cast<UMVVMView>(GetExtension(UMVVMView::StaticClass()));
		auto* DragMVVMView = Cast<UMVVMView>(NewDragUserWidget->GetExtension(UMVVMView::StaticClass()));
		if (IsValid(OwnMVVMView) == true && IsValid(DragMVVMView) == true)
		{
			auto* OwnViewModel = Cast<UMVVMViewModelBase>(OwnMVVMView->GetViewModel(mDragViewModelName).GetObject());
			if (IsValid(OwnViewModel) == true)
			{
				DragMVVMView->SetViewModel(mDragViewModelName, OwnViewModel);
			}
			auto* OwnViewHandler = Cast<IDragDropViewHandler>(OwnViewModel);
			if (OwnViewHandler != nullptr)
			{
				OwnViewHandler->OnBeginDrag();
			}
		}

		OutOperation->DefaultDragVisual = NewDragUserWidget;
		PlaySound(mDragDetectSound);
	}
}

void UDragDropWidgetBase::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	auto* OwnMVVMView = Cast<UMVVMView>(GetExtension(UMVVMView::StaticClass()));
	if (IsValid(OwnMVVMView) == true)
	{
		auto* OwnViewHandler = Cast<IDragDropViewHandler>(OwnMVVMView->GetViewModel(mDragViewModelName).GetObject());
		if (OwnViewHandler != nullptr)
		{
			OwnViewHandler->OnEndDrag();
		}
	}
}

void UDragDropWidgetBase::SetCanDrag(bool bWillCanDrag)
{
	bCanDrag = bWillCanDrag;
}

bool UDragDropWidgetBase::CanDrag() const
{
	return bCanDrag;
}
