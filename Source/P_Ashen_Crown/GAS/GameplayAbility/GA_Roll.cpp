// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_Roll.h"
#include "../AttributeSet/PlayerAttributeSet.h"
#include "../GameplayEffect/GE_StaminaCost_Roll.h"
#include "../GameplayEffect/GE_StaminaRegenDelay.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGA_Roll::UGA_Roll()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RollMontage(TEXT("/Script/Engine.AnimMontage'/Game/AC/Blueprint/Anim/Player/Nyx/AM_NyxRoll.AM_NyxRoll'"));

	if (RollMontage.Succeeded())
		mRollMontage = RollMontage.Object;

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	FAbilityTriggerData	TriggerData;

	TriggerData.TriggerTag = AC_PlayerState::Player_State_Rolling.GetTag();

	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AbilityTriggers.Add(TriggerData);

	ActivationOwnedTags.AddTag(AC_PlayerState::Player_State_Rolling.GetTag());
	
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_Combat.GetTag());
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_State_Healing.GetTag());
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_State_Death.GetTag());
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_Skill_Active01.GetTag());
}

bool UGA_Roll::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (ActorInfo && ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(AC_PlayerState::Player_State_Rolling.GetTag()))
	{
		return false;
	}

	if (ActorInfo->AbilitySystemComponent->GetSet<UPlayerAttributeSet>() && ActorInfo->AbilitySystemComponent->GetSet<UPlayerAttributeSet>()->GetStamina() < mStaminaCost)
	{
		return false;
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Start Failed"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	UPlayerAttributeSet* AttrSet = const_cast<UPlayerAttributeSet*>(SourceASC->GetSet<UPlayerAttributeSet>());

	FGameplayEffectSpecHandle StaminaCostSpec = MakeOutgoingGameplayEffectSpec(UGE_StaminaCost_Roll::StaticClass(), GetAbilityLevel());

	FGameplayEffectSpecHandle RegenDelaySpec = MakeOutgoingGameplayEffectSpec(UGE_StaminaRegenDelay::StaticClass(), GetAbilityLevel()); 
	
	RegenDelaySpec.Data->SetSetByCallerMagnitude(AC_PlayerState::Player_State_StaminaRegenDelay.GetTag(), mCoolDown);
	RegenDelaySpec.Data->DynamicGrantedTags.AddTag(AC_PlayerState::Player_State_StaminaRegenDelay.GetTag());
	SourceASC->ApplyGameplayEffectSpecToSelf(*RegenDelaySpec.Data);

	if (StaminaCostSpec.IsValid())
	{
		StaminaCostSpec.Data->SetSetByCallerMagnitude(AC_PlayerState::Data_Rolling_Cost.GetTag(), -mStaminaCost);

		SourceASC->ApplyGameplayEffectSpecToSelf(*StaminaCostSpec.Data);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NOT StaminaCostSpec"));
	}

	if (mRollMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, NAME_None, mRollMontage, 1.0f, NAME_None, true, 1.0f);

		PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Roll::OnMontageEnded);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Roll::OnMontageEnded);
		PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Roll::OnMontageEnded);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &UGA_Roll::OnMontageEnded);
		PlayMontageTask->Activate();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("false RollMontage"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
}

void UGA_Roll::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(AC_PlayerState::Player_State_Rolling.GetTag());
	}
}

void UGA_Roll::OnMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
