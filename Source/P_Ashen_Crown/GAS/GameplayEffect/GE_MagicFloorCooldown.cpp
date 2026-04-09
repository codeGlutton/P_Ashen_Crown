// Fill out your copyright notice in the Description page of Project Settings.


#include "GE_MagicFloorCooldown.h"

UGE_MagicFloorCooldown::UGE_MagicFloorCooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat SetByCaller;
	SetByCaller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Monster.Data.CoolDown.MagicFloor"));

	DurationMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);

	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.MagicFloorSkill")));
}