// Fill out your copyright notice in the Description page of Project Settings.


#include "GE_Skill01Cooldown.h"

UGE_Skill01Cooldown::UGE_Skill01Cooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat	SetByCallerFloat;
	SetByCallerFloat.DataTag = AC_PlayerState::Data_Skill_01Cooldown.GetTag();

	DurationMagnitude = FGameplayEffectModifierMagnitude(SetByCallerFloat);
}
