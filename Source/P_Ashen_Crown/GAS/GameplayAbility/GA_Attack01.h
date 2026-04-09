// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GASCore.h"
#include "../BaseGameplayAbility.h"
#include "GA_Attack01.generated.h"

/**
 * 이건 첫번째 공격
 */
UCLASS()
class P_ASHEN_CROWN_API UGA_Attack01 : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Attack01();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage>	mAttack01Montage; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Tags")
	FGameplayTagContainer		mActivationTags;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void OnMontageEnded();
};
