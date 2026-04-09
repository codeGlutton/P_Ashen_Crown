// Fill out your copyright notice in the Description page of Project Settings.


#include "GE_Heal.h"
#include "../../GAS/AttributeSet/PlayerAttributeSet.h"

UGE_Heal::UGE_Heal()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo	Modifier;
	Modifier.Attribute = UPlayerAttributeSet::GetHPAttribute();
	Modifier.ModifierOp = EGameplayModOp::Additive;
	Modifier.ModifierMagnitude = FScalableFloat(100.f);

	Modifiers.Add(Modifier);
}
