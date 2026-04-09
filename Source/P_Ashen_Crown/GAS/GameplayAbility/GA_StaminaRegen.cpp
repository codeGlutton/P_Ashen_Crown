// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_StaminaRegen.h"
#include "../AttributeSet/PlayerAttributeSet.h"

UGA_StaminaRegen::UGA_StaminaRegen()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_StaminaRegen::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	GetWorld()->GetTimerManager().SetTimer(mRegenTickTimer, this, &UGA_StaminaRegen::RegenTick, mTickInterval, true);
}

void UGA_StaminaRegen::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetWorld()->GetTimerManager().ClearTimer(mRegenTickTimer);
}

void UGA_StaminaRegen::RegenTick()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	UPlayerAttributeSet* AttrSet = const_cast<UPlayerAttributeSet*>(ASC->GetSet<UPlayerAttributeSet>());

	if (!AttrSet)
	{
		return;
	}

	const float CurrentStamina = AttrSet->GetStamina();
	const float MaxStamina = AttrSet->GetStaminaMax();

	if (ASC->HasMatchingGameplayTag(AC_PlayerState::Player_State_StaminaRegenDelay.GetTag()))
	{
		return;
	}

	if (CurrentStamina < MaxStamina)
	{
		const float DeltaRegen = mRegenRatePerSecond * mTickInterval;

		//MaxStamina를 가끔 조금 넘겨서까지 회복하는것을 방지하기위함
		const float NewStamina = FMath::Clamp(CurrentStamina + DeltaRegen, 0.0f, MaxStamina);
		const float FinalDelta = NewStamina - CurrentStamina;

		ASC->ApplyModToAttribute(AttrSet->GetStaminaAttribute(), EGameplayModOp::Additive, FinalDelta);
	}
	/*else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}*/
}