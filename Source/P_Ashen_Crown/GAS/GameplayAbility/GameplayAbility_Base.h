// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GASCore.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Base.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UGameplayAbility_Base : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_Base();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float		mCoolDown = 0.f;

	bool		mAbilityActive = false;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	
public:
	const TArray<FAbilityTriggerData>& GetAbilityTriggers()	const
	{
		return AbilityTriggers;
	}	
};
