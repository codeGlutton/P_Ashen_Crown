// Fill out your copyright notice in the Description page of Project Settings.


#include "GE_StaminaRegenDelay.h"

UGE_StaminaRegenDelay::UGE_StaminaRegenDelay()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat	SetByCallerFloat;
	SetByCallerFloat.DataTag = AC_PlayerState::Player_State_StaminaRegenDelay.GetTag();

	DurationMagnitude = FGameplayEffectModifierMagnitude(SetByCallerFloat);

	StackingType = EGameplayEffectStackingType::AggregateBySource;
	StackLimitCount = 1;
	StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::RefreshOnSuccessfulApplication;
	StackPeriodResetPolicy = EGameplayEffectStackingPeriodPolicy::ResetOnSuccessfulApplication;
	StackExpirationPolicy = EGameplayEffectStackingExpirationPolicy::ClearEntireStack;
}
