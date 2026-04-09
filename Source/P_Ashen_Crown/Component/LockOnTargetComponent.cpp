#include "LockOnTargetComponent.h"
#include "Blueprint/UserWidget.h"

ULockOnTargetComponent::ULockOnTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULockOnTargetComponent::RegisterView(ILockOnViewHandler* Handler, UUserWidget* Widget)
{
	mLockOnHandler.SetObject(Handler->_getUObject());
	mLockOnHandler.SetInterface(Handler);
	mLockOnWidget = Widget;
}

void ULockOnTargetComponent::UnregisterView()
{
	CloseLockOnView();
	mLockOnHandler.SetObject(nullptr);
	mLockOnHandler.SetInterface(nullptr);
	mLockOnWidget = nullptr;
}

void ULockOnTargetComponent::OpenLockOnView(AActor* Instigator, APlayerController* PlayerController)
{
	mCurInstigator = Instigator;
	mCurPlayerController = PlayerController;
	if (mLockOnHandler.GetInterface() != nullptr)
	{
		mLockOnHandler->ShowView(this);
	}
}

void ULockOnTargetComponent::CloseLockOnView()
{
	if (mLockOnHandler.GetInterface() != nullptr)
	{
		mLockOnHandler->CloseView();
	}
	mCurInstigator = nullptr;
	mCurPlayerController = nullptr;
}