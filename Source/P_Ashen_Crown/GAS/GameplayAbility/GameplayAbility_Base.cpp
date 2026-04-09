// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_Base.h"
#include "../BaseAttributeSet.h"
#include "../AttributeSet/MonsterAttributeSet.h"

UGameplayAbility_Base::UGameplayAbility_Base()
{
}

void UGameplayAbility_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	mAbilityActive = true;

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Start Failed"));
		mAbilityActive = false;
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	AActor* SourceActor = GetAvatarActorFromActorInfo();

	// AbilitySystem을 가져온다.
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

	// AttributeSet을 얻어온다.
	UMonsterAttributeSet* SourceAttr = const_cast<UMonsterAttributeSet*>(SourceASC->GetSet<UMonsterAttributeSet>());

	if (mCoolDown > 0.f)
	{
		// 쿨다운 처리
		float CoolDown = mCoolDown;

		if (SourceAttr)
		{
			// 쿨감 20%일 경우
			CoolDown *= (1.f - 0.2f);
		}

		// 쿨다운 계산.
		//FGameplayEffectSpecHandle	CoolDownSpec = MakeOutgoingGameplayEffectSpec(UGameplayEffect_CoolDown::StaticClass(),
		//	GetAbilityLevel());

		//// 쿨다운 태그를 동적으로 적용한다.
		//CoolDownSpec.Data->DynamicGrantedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.Skill1")));

		//// Cool Down 적용
		//SourceASC->ApplyGameplayEffectSpecToSelf(*CoolDownSpec.Data);
	}
}
