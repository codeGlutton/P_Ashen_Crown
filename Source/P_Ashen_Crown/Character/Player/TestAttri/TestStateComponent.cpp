// Fill out your copyright notice in the Description page of Project Settings.


#include "TestStateComponent.h"

UTestStateComponent::UTestStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTestStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTestStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTestStateComponent::SetState(const FGameplayTag NewState)
{
	mCurrentState = NewState;
}

FGameplayTag UTestStateComponent::GetCurrentState() const
{
	return mCurrentState;
}

bool UTestStateComponent::IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const
{
	return TagsToCheck.HasTagExact(mCurrentState);
}

bool UTestStateComponent::MovementInputEnabled() const
{
	return bMovementInputEnabled;
}

void UTestStateComponent::ToggleMovementInput(bool bEnabled, float Duration)
{
	if (bEnabled)
	{
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		LatentAction.ExecutionFunction = "MovementInputEnableAction";
		LatentAction.Linkage = 0;
		LatentAction.UUID = 0;

		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), Duration, LatentAction);
	}
	else
	{
		bMovementInputEnabled = false;
	}
}

void UTestStateComponent::MovementInputEnableAction()
{
	bMovementInputEnabled = true;
}
