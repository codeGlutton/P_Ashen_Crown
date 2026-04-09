// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayEffect_CoolDown.h"

UGameplayEffect_CoolDown::UGameplayEffect_CoolDown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat	SetByCallerFloat;
	SetByCallerFloat.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Monster.Data.CoolDown"));

	DurationMagnitude = FGameplayEffectModifierMagnitude(SetByCallerFloat);

	// 이렇게 하면 Skill1 의 전용 쿨다운이 된다.
	CachedGrantedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.Skill1")));
}
