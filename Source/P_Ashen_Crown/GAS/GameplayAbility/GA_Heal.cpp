// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Heal.h"
#include "../../Character/Player/Interface/PotionProvider.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGA_Heal::UGA_Heal()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HealMontage(TEXT("/Script/Engine.AnimMontage'/Game/AC/Blueprint/Anim/Player/Nyx/AM_NyxHeal.AM_NyxHeal'"));

	if (HealMontage.Succeeded())
		mHealMontage = HealMontage.Object;

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	FAbilityTriggerData	TriggerData;

	TriggerData.TriggerTag = AC_PlayerState::Player_State_Healing.GetTag();

	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AbilityTriggers.Add(TriggerData);

	ActivationOwnedTags.AddTag(AC_PlayerState::Player_State_Healing.GetTag());
	
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_Combat.GetTag());
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_State_Rolling.GetTag());
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_Skill_Active01.GetTag());
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_State_Death.GetTag());
}

bool UGA_Heal::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (ActorInfo && ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(AC_PlayerState::Player_State_Healing.GetTag()))
	{
		return false;
	}

	AActor* Avatar = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
	const IPotionProvider* Provider = Cast<IPotionProvider>(Avatar);
	if (!Provider || !Provider->HasPotion())
	{
		UE_LOG(LogTemp, Warning, TEXT("포션이 없어용"));
		return false;
	}
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_Heal::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Start Failed"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	// mActivationTags.AddTag(AC_PlayerState::Player_State_Healing.GetTag());
	// ActorInfo->AbilitySystemComponent->AddLooseGameplayTags(mActivationTags);

	if (mHealMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, NAME_None, mHealMontage, 1.0f, NAME_None, true, 1.0f);

		PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Heal::OnMontageEnded);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Heal::OnMontageEnded);
		PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Heal::OnMontageEnded);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &UGA_Heal::OnMontageEnded);
		PlayMontageTask->Activate();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("false RollMontage"));

		ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTags(mActivationTags);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
}

void UGA_Heal::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(AC_PlayerState::Player_State_Healing.GetTag());
	}
}

void UGA_Heal::OnMontageEnded()
{
	AActor* Avatar = CurrentActorInfo ? CurrentActorInfo->AvatarActor.Get() : nullptr;
	IPotionProvider* Provider = Cast<IPotionProvider>(Avatar);
	if (Provider)
	{
		const bool bSuccess = Provider->ConsumePotion();
		if (!bSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("포션 제거 실패"));
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
