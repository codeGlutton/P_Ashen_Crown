#include "NPCAnimInstance.h"

void UNPCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	auto* ASCInterface = Cast<IAbilitySystemInterface>(GetOwningActor());
	if (ASCInterface != nullptr)
	{
		mOwnerASC = ASCInterface->GetAbilitySystemComponent();
		if (IsValid(mOwnerASC) == true)
		{
			mOwnerTagContainer = mOwnerASC->GetOwnedGameplayTags();
			mOwnerASC->RegisterGameplayTagEvent(AC_NPCEmotion::NPC_Emotion).AddUObject(this, &UNPCAnimInstance::UpdateTags);
		}
	}
}

void UNPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto* OwnerPawn = Cast<APawn>(GetOwningActor());
	if (IsValid(OwnerPawn) == true)
	{
		mMoveVelocity = OwnerPawn->GetMovementComponent()->Velocity;
		mForwardVelocity = FVector::DotProduct(mMoveVelocity, OwnerPawn->GetActorForwardVector());
		mRightVelocity = FVector::DotProduct(mMoveVelocity, OwnerPawn->GetActorRightVector());
	}
}

void UNPCAnimInstance::UpdateTags(const FGameplayTag GameplayTag, int32 Count)
{
	if (IsValid(mOwnerASC) == true)
	{
		mOwnerTagContainer = mOwnerASC->GetOwnedGameplayTags();
	}
}
