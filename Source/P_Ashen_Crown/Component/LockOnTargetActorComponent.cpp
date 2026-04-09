#include "LockOnTargetActorComponent.h"
#include "P_Ashen_Crown/Component/CameraTargetingBlendComponent.h"

void ULockOnTargetActorComponent::OpenLockOnView(AActor* Instigator, APlayerController* PlayerController)
{
	Super::OpenLockOnView(Instigator, PlayerController);

	auto* CameraTargetingBlendComp = mCurPlayerController->FindComponentByClass<UCameraTargetingBlendComponent>();
	if (IsValid(CameraTargetingBlendComp) == false)
	{
		return;
	}
	CameraTargetingBlendComp->SetTargetActor(GetOwner());
}

void ULockOnTargetActorComponent::CloseLockOnView()
{
	auto* CameraTargetingBlendComp = mCurPlayerController->FindComponentByClass<UCameraTargetingBlendComponent>();
	if (IsValid(CameraTargetingBlendComp) == false)
	{
		return;
	}
	CameraTargetingBlendComp->SetTargetActor(nullptr);

	Super::CloseLockOnView();
}
