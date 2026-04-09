#include "InteractableTargetComponent.h"
#include "Blueprint/UserWidget.h"

/************************
UInteractableTargetComponent
*************************/

UInteractableTargetComponent::UInteractableTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractableTargetComponent::RegisterView(IInteractableViewHandler* Handler, UUserWidget* Widget, const FText& DisplayText)
{
	mInteractableHandler.SetObject(Handler->_getUObject());
	mInteractableHandler.SetInterface(Handler);
	mInteractWidget = Widget;
	mDisplayText = DisplayText;
}

void UInteractableTargetComponent::UnregisterView()
{
	CloseInteractView();
	mInteractableHandler.SetObject(nullptr);
	mInteractableHandler.SetInterface(nullptr);
	mInteractWidget = nullptr;
}

void UInteractableTargetComponent::UpdateDisplayText(const FText& DisplayText)
{
	mDisplayText = DisplayText;
}

void UInteractableTargetComponent::OpenInteractView(AActor* Instigator)
{
	if (mInteractableHandler.GetInterface() == nullptr)
	{
		return;
	}
	mCurInstigator = Instigator;
	mInteractableHandler->SetDisplayText(mDisplayText);
	mInteractWidget->AddToViewport(-1);
	mInteractableHandler->ShowView(this);
}

void UInteractableTargetComponent::CloseInteractView()
{
	if (mInteractableHandler.GetInterface() == nullptr)
	{
		return;
	}

	mInteractableHandler->CloseView();
	mCurInstigator = nullptr;
}

void UInteractableTargetComponent::Interact(UObject* Payload)
{
	OnPreInteract.Broadcast(mCurInstigator);
}
