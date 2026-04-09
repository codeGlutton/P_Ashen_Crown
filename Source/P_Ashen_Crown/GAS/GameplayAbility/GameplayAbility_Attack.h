// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayAbility_Base.h"
#include "GameplayAbility_Attack.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UGameplayAbility_Attack : public UGameplayAbility_Base
{
	GENERATED_BODY()
	
public:
	UGameplayAbility_Attack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
};
