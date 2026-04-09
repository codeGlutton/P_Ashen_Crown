// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Attack02.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemBlueprintLibrary.h"

UGA_Attack02::UGA_Attack02()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack2Montage(TEXT("/Script/Engine.AnimMontage'/Game/AC/Blueprint/Anim/Player/Nyx/AM_NyxAttack02.AM_NyxAttack02'"));
	
	if (Attack2Montage.Succeeded())
		mAttack02Montage = Attack2Montage.Object;
	
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	
	FAbilityTriggerData	TriggerData;
	
	TriggerData.TriggerTag = AC_PlayerState::Player_Combat_Attacking2.GetTag();
	
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	
	AbilityTriggers.Add(TriggerData);

	ActivationOwnedTags.AddTag(AC_PlayerState::Player_Combat_Attacking2.GetTag());
	
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_State_Rolling.GetTag());
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_Skill_Active01.GetTag());
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_State_Healing.GetTag());
	ActivationBlockedTags.AddTag(AC_PlayerState::Player_Combat_Attacking2.GetTag());
}

void UGA_Attack02::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Start Failed"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	/*if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}*/
	
	if (mAttack02Montage)
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, NAME_None, mAttack02Montage, 1.0f, NAME_None, true, 1.0f);
	
		PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Attack02::OnMontageEnded);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Attack02::OnMontageEnded);
		PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Attack02::OnMontageEnded);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &UGA_Attack02::OnMontageEnded);
		PlayMontageTask->Activate();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
}

void UGA_Attack02::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(AC_PlayerState::Player_Combat_Attacking2.GetTag());
	}
}

void UGA_Attack02::OnMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
