// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GAS/GASCore.h"
#include "../BaseGameplayAbility.h"
#include "GA_StaminaRegen.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UGA_StaminaRegen : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_StaminaRegen();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Regen")
	float			mRegenRatePerSecond = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Regen")
	float			mTickInterval = 0.2f;

	FTimerHandle	mRegenTickTimer;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void RegenTick();
};
