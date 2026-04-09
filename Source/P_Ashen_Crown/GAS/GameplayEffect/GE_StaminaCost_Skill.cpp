// Fill out your copyright notice in the Description page of Project Settings.


#include "GE_StaminaCost_Skill.h"
#include "../AttributeSet/PlayerAttributeSet.h"

UGE_StaminaCost_Skill::UGE_StaminaCost_Skill()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo	Modifier;
	Modifier.Attribute = UPlayerAttributeSet::GetStaminaAttribute();
	Modifier.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat	SetByCallerFloat;
	SetByCallerFloat.DataTag = AC_PlayerState::Data_Rolling_Cost.GetTag();

	Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCallerFloat);
	Modifiers.Add(Modifier);
}
