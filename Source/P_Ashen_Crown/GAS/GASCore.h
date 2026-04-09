// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameCore.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h" 
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffect.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

namespace AC_PlayerState
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Rolling);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_StaminaRegenDelay);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Healing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Hit);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Death);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_Active01);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Combat);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Combat_Attacking1);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Combat_Attacking2);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Combat_Attacking3);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Combat_Attacking4);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Rolling_Cost);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Attacking_Cost);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Skill01_01Cost);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_LoadData_HP);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Skill_01Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_NormalAttack_Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Skill01_Damage);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_ComboWindow_Open);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_ComboWindow_2);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_ComboWindow_3);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_AttackInput_Block);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_ComboWindow_Close);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Player_Attack_BP_NormalAttackCue);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Player_Skill);
}

namespace AC_NPCEmotion
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(NPC_Emotion);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(NPC_Emotion_None);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(NPC_Emotion_Happy);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(NPC_Emotion_Scared);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(NPC_Emotion_Disappointed);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(NPC_Emotion_Tired);
}