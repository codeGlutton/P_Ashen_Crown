// Fill out your copyright notice in the Description page of Project Settings.


#include "GE_ProjectileSkillCooldown.h"

UGE_ProjectileSkillCooldown::UGE_ProjectileSkillCooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat SetByCaller;
	SetByCaller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Monster.Data.CoolDown.Projectile"));

	DurationMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);

	// 쿨타임 태그 부여
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.ProjectileSkill")));
}
