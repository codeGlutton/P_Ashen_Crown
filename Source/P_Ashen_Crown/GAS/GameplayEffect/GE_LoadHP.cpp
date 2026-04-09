#include "GE_LoadHP.h"
#include "../../GAS/BaseAttributeSet.h"

UGE_LoadHP::UGE_LoadHP()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo	Modifier;
	Modifier.ModifierOp = EGameplayModOp::Override;
	Modifier.Attribute = UBaseAttributeSet::GetHPAttribute();

	FSetByCallerFloat	SetByCallerFloat;
	SetByCallerFloat.DataTag = AC_PlayerState::Data_LoadData_HP.GetTag();

	Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCallerFloat);
	Modifiers.Add(Modifier);
}