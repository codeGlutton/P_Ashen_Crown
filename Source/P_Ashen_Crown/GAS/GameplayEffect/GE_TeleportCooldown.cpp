// Fill out your copyright notice in the Description page of Project Settings.


#include "GE_TeleportCooldown.h"

UGE_TeleportCooldown::UGE_TeleportCooldown()
{
    DurationPolicy = EGameplayEffectDurationType::HasDuration;

    FSetByCallerFloat SetByCaller;
    SetByCaller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Monster.Data.CoolDown.Teleport"));

    DurationMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);

    InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.TeleportSkill")));
}
