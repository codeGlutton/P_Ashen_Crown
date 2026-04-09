// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Skill01.h"
#include "../AttributeSet/PlayerAttributeSet.h"
#include "../GameplayEffect/GE_Skill01Cooldown.h"
#include "../GameplayEffect/GE_StaminaCost_Skill.h"
#include "../GameplayEffect/GE_StaminaRegenDelay.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGA_Skill01::UGA_Skill01()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Skill01Montage(TEXT("/Script/Engine.AnimMontage'/Game/AC/Blueprint/Anim/Player/Nyx/AM_NyxSkill.AM_NyxSkill'"));

	if (Skill01Montage.Succeeded())
		mSkill01Montage = Skill01Montage.Object;

	static ConstructorHelpers::FClassFinder<UGameplayEffect> CooldownEffectClass(TEXT("/Script/Engine.Blueprint'/Game/AC/Blueprint/GAS/Effect/BP_Skill01_Cooldown.BP_Skill01_Cooldown_C'"));

	if (CooldownEffectClass.Succeeded())
		mCooldownEffect = CooldownEffectClass.Class;

	ActivationOwnedTags.AddTag(AC_PlayerState::Player_Skill_Active01.GetTag());
	
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_State_Rolling.GetTag());
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_Skill_Active01.GetTag());
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_State_Healing.GetTag());
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_State_Death.GetTag());
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_Combat_Attacking1.GetTag());
}

bool UGA_Skill01::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (ActorInfo && ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(AC_PlayerState::Data_Skill_01Cooldown.GetTag()))
	{
		return false;
	}

	if (ActorInfo->AbilitySystemComponent->GetSet<UPlayerAttributeSet>() && ActorInfo->AbilitySystemComponent->GetSet<UPlayerAttributeSet>()->GetStamina() < mStaminaCost)
	{
		return false;
	}
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_Skill01::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                 const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

	FGameplayEffectSpecHandle	CoolDownSpec = MakeOutgoingGameplayEffectSpec(UGE_Skill01Cooldown::StaticClass(), GetAbilityLevel());
	FGameplayEffectSpecHandle StaminaCostSpec = MakeOutgoingGameplayEffectSpec(UGE_StaminaCost_Skill::StaticClass(), GetAbilityLevel());
	FGameplayEffectSpecHandle RegenDelaySpec = MakeOutgoingGameplayEffectSpec(UGE_StaminaRegenDelay::StaticClass(), GetAbilityLevel()); 

	CoolDownSpec.Data->SetSetByCallerMagnitude(AC_PlayerState::Data_Skill_01Cooldown.GetTag(), mSkillCoolDown);
	CoolDownSpec.Data->DynamicGrantedTags.AddTag(AC_PlayerState::Data_Skill_01Cooldown.GetTag());
	
	if (CoolDownSpec.IsValid())
	{
		SourceASC->ApplyGameplayEffectSpecToSelf(*CoolDownSpec.Data);
	}
	
	RegenDelaySpec.Data->SetSetByCallerMagnitude(AC_PlayerState::Player_State_StaminaRegenDelay.GetTag(), mStaminaCoolDown);
	RegenDelaySpec.Data->DynamicGrantedTags.AddTag(AC_PlayerState::Player_State_StaminaRegenDelay.GetTag());
	SourceASC->ApplyGameplayEffectSpecToSelf(*RegenDelaySpec.Data);
	
	if (StaminaCostSpec.IsValid())
	{
		StaminaCostSpec.Data->SetSetByCallerMagnitude(AC_PlayerState::Data_Rolling_Cost.GetTag(), -mStaminaCost);

		SourceASC->ApplyGameplayEffectSpecToSelf(*StaminaCostSpec.Data);
	}

	
	if (mSkill01Montage)
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, NAME_None, mSkill01Montage, 1.0f, NAME_None, true, 1.0f);
	
		PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Skill01::OnMontageEnded);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Skill01::OnMontageEnded);
		PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Skill01::OnMontageEnded);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &UGA_Skill01::OnMontageEnded);
		PlayMontageTask->Activate();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
}

bool UGA_Skill01::CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags)
{
	return Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
}

void UGA_Skill01::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                             const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(AC_PlayerState::Player_Skill_Active01.GetTag());
	}
}

void UGA_Skill01::OnMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
