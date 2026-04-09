// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_PeriodicDamage.h"
#include "AbilitySystemGlobals.h"
#include "../BaseAttributeSet.h"

UAbilityTask_PeriodicDamage::UAbilityTask_PeriodicDamage()
{
}

UAbilityTask_PeriodicDamage* UAbilityTask_PeriodicDamage::ApplyPeriodicDamage(UGameplayAbility* OwningAbility,
	const FVector& Origin, float Radius, float Interval, float Duration, float Damage, 
	TSubclassOf<UGameplayEffect> DamageEffect)
{
	UAbilityTask_PeriodicDamage* Task = NewAbilityTask<UAbilityTask_PeriodicDamage>(OwningAbility);

	Task->mEffectOrigin = Origin;
	Task->mEffectRadius = Radius;
	Task->mTickInterval = Interval;
	Task->mTotalDuration = Duration;
	Task->mDamage = Damage;
	Task->mDamageEffect = DamageEffect;

	return Task;
}


void UAbilityTask_PeriodicDamage::Activate()
{
	if (Ability == nullptr || !mDamageEffect)
	{
		// Task를 끝낸다.
		EndTask();
		return;
	}

	// 데미지 첫번째 틱 실행.
	ApplyDamage();

	//// 타이머 생성.
	GetWorld()->GetTimerManager().SetTimer(mTimerHandle, this, &UAbilityTask_PeriodicDamage::ApplyDamage,
		mTickInterval, true);
}

void UAbilityTask_PeriodicDamage::OnDestroy(bool bInOwnerFinished)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(mTimerHandle);
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_PeriodicDamage::ApplyDamage()
{
	mTimeElapsed += mTickInterval;
	if (mTimeElapsed > mTotalDuration)
	{
		mOnFinished.Broadcast();
		EndTask();
		return;
	}

	TArray<FHitResult> HitArray;
	FCollisionShape Shape = FCollisionShape::MakeSphere(mEffectRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetAvatarActor());

	GetWorld()->SweepMultiByChannel(HitArray, mEffectOrigin, mEffectOrigin, FQuat::Identity,
		ECC_Pawn, Shape, Params);

	for (const auto& Hit : HitArray)
	{
		AActor* Target = Hit.GetActor();
		if (!Target || Target == GetAvatarActor())
		{
			continue;
		}

		UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);

		if (SourceASC)
		{
			// 이펙트 컨텍스트 핸들 생성
			FGameplayEffectSpecHandle SpecHandle = Ability->MakeOutgoingGameplayEffectSpec(mDamageEffect,
				Ability->GetAbilityLevel());

			if (SpecHandle.IsValid())
			{
				FGameplayEffectContextHandle	Context = Ability->MakeEffectContext(Ability->GetCurrentAbilitySpecHandle(),
					Ability->GetCurrentActorInfo());

				Context.AddHitResult(Hit);

				SpecHandle.Data->SetContext(Context);

				AActor* TargetActor = Hit.GetActor();

				UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

				if (!TargetASC)
				{
					mOnFinished.Broadcast();
					EndTask();
					return;
				}

				// Damage를 구한다.
				float	SourceAttack = SourceASC->GetSet<UBaseAttributeSet>()->GetAttackPower();
				float	TargetDefense = TargetASC->GetSet<UBaseAttributeSet>()->GetDefense();

				float	Dmg = SourceAttack - TargetDefense;
				Dmg = Dmg < 1.f ? 1.f : Dmg;

				// 이 태그를 사용하는 SetByCaller에 지정할 값을 설정한다.
				SpecHandle.Data->SetSetByCallerMagnitude(
					FGameplayTag::RequestGameplayTag(TEXT("Monster.Data.Damage")),
					-Dmg);

				TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}
