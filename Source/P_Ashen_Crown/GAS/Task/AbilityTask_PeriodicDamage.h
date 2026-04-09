// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GASCore.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_PeriodicDamage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPeriodicDamageFinished);

UCLASS()
class P_ASHEN_CROWN_API UAbilityTask_PeriodicDamage : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UAbilityTask_PeriodicDamage();

private:
	TSubclassOf<UGameplayEffect>	mDamageEffect;
	FVector	mEffectOrigin;
	float	mEffectRadius = 0.f;
	float	mTickInterval = 0.f;
	float	mTotalDuration = 0.f;
	float	mTimeElapsed = 0.f;
	float	mDamage = 0.f;
	FTimerHandle	mTimerHandle;

public:
	FPeriodicDamageFinished	mOnFinished;

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "ApplyPeriodicDamage", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UAbilityTask_PeriodicDamage* ApplyPeriodicDamage(UGameplayAbility* OwningAbility,
		const FVector& Origin, float Radius, float Interval, float Duration, float Damage, 
		TSubclassOf<UGameplayEffect> DamageEffect);

protected:
	virtual void Activate();
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void ApplyDamage();
};
