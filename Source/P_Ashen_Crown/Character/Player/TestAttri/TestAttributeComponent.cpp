// Fill out your copyright notice in the Description page of Project Settings.


#include "TestAttributeComponent.h"

UTestAttributeComponent::UTestAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTestAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTestAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UTestAttributeComponent::CheckHasEnoughStamina(float StaminaCost) const
{
	return mBaseStamina >= StaminaCost;
}

void UTestAttributeComponent::DecreaseStamina(float StaminaCost)
{
	mBaseStamina = FMath::Clamp(mBaseStamina - StaminaCost, 0.f, mMaxStamina);
}

void UTestAttributeComponent::ToggleStaminaRegeneration(bool bEnabled, float StartDelay)
{
	if (bEnabled)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(mStaminaRegenTimerHandle) == false)
		{
			GetWorld()->GetTimerManager().SetTimer(mStaminaRegenTimerHandle, this, &ThisClass::RegenerateStaminaHandle, 0.1f, true, StartDelay);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(mStaminaRegenTimerHandle);
	}
}

void UTestAttributeComponent::RegenerateStaminaHandle()
{
	mBaseStamina = FMath::Clamp(mBaseStamina + mRegenrateStamina, 0.f, mMaxStamina);

	if (mBaseStamina >= mMaxStamina)
	{
		ToggleStaminaRegeneration(false);
	}
}

