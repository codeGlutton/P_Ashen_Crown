// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GAS/GASCore.h"
#include "../BaseGameplayAbility.h"
#include "GA_Roll.generated.h"

class UPlayerAttributeSet;

UCLASS()
class P_ASHEN_CROWN_API UGA_Roll : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Roll();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage>			mRollMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Tags")
	FGameplayTagContainer				mActivationTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float		mStaminaCost = 25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float		mCoolDown = 3.f;

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void OnMontageEnded();
};
