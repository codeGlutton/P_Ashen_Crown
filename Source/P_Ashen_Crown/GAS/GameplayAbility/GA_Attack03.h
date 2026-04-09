// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GAS/GASCore.h"
#include "../BaseGameplayAbility.h"
#include "GA_Attack03.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UGA_Attack03 : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Attack03();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage>	mAttack03Montage;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	UFUNCTION()
	void OnMontageEnded();
};
