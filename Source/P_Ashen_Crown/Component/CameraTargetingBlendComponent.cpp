#include "CameraTargetingBlendComponent.h"

UCameraTargetingBlendComponent::UCameraTargetingBlendComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 60프레임으로 조정
	PrimaryComponentTick.TickInterval = 0.016f;
}

void UCameraTargetingBlendComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(mTargetActor) == false)
	{
		mTargetActor = nullptr;
		return;
	}
	APlayerController* PlayerController = GetOwner<APlayerController>();
	if (IsValid(PlayerController) == false)
	{
		return;
	}
	AActor* Pawn = PlayerController->GetPawn();
	if (IsValid(Pawn) == false)
	{
		return;
	}

	FRotator CurrentRot = PlayerController->GetControlRotation();
	FRotator TargetRot = (mTargetActor->GetActorLocation() - Pawn->GetActorLocation()).Rotation();
	TargetRot.Pitch = 0.f;
	TargetRot.Roll = 0.f;

	PlayerController->SetControlRotation(FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 5.0f));
}

void UCameraTargetingBlendComponent::SetTargetActor(AActor* TargetActor)
{
	mTargetActor = TargetActor;
}
