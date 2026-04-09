#include "LockOnTargetCameraComponent.h"

ULockOnTargetCameraComponent::ULockOnTargetCameraComponent() :
	mBlendTime(1.f)
{
}

void ULockOnTargetCameraComponent::OpenLockOnView(AActor* Instigator, APlayerController* PlayerController)
{
	Super::OpenLockOnView(Instigator, PlayerController);

	auto* TargetCameraComp = GetOwner()->FindComponentByClass<UCameraComponent>();
	if (IsValid(TargetCameraComp) == true)
	{
		mCurPlayerController->SetViewTargetWithBlend(GetOwner(), mBlendTime);
	}
}

void ULockOnTargetCameraComponent::CloseLockOnView()
{
	mCurPlayerController->SetViewTargetWithBlend(mCurPlayerController->GetPawn(), mBlendTime);
	Super::CloseLockOnView();
}

void ULockOnTargetCameraComponent::SetBlendTime(float BlendTime)
{
	mBlendTime = BlendTime;
}
