// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_Attack.h"
#include "../GameplayEffect/GameplayEffect_Damage.h"
#include "../BaseAttributeSet.h"

UGameplayAbility_Attack::UGameplayAbility_Attack()
{
	// 어빌리티 객체의 인스턴스가 어떻게 관리되는지를 결정하는 값이다.
	// NonInstanced : 단순효과, 쿨타임 없는 효과에서 주로 사용.
	// InstancedPerAcrtor : 공유 인스턴스 활용. 무기교체, 지속 버프 등에서 사용.
	// InstancedPerExecution : 매번 생성. 동시 실행되는 공격, 도트 데미지 등에서 활용.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	// 타겟을 얻어오기 위해 이벤트를 설정한다.
	FAbilityTriggerData	TriggerData;

	// 사용할 태그를 지정한다. 이 태그를 동작시키면 이 어빌리티가 동작된다.
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("Monster.Ability.Attack"));

	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AbilityTriggers.Add(TriggerData);
}

void UGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo,
		TriggerEventData);

	if (!mAbilityActive)
	{
		return;
	}

	else if (!TriggerEventData || !TriggerEventData->Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Failed"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 공격시 지정한 타겟 정보를 얻어온다.
	FGameplayAbilityTargetData_SingleTargetHit* HitData =
		static_cast<FGameplayAbilityTargetData_SingleTargetHit*>(TriggerEventData->TargetData.Data[0].Get());

	if (!HitData)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 이 어빌리티를 발생시킨 Actor의 ASC를 얻어온다.
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

	// Target의 ASC를 얻어온다.
	AActor* TargetActor = HitData->HitResult.GetActor();
	//AActor* Actor = ActorInfo->OwnerActor.Get();

	UAbilitySystemComponent* TargetASC = 
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (!SourceASC || !TargetASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 데미지를 주기 위해 Damage GameplayEffect를 생성하고 동작시킨다.
	// 이펙트 컨텍스트 핸들을 생성한다.
	FGameplayEffectContextHandle	Context = MakeEffectContext(Handle, ActorInfo);

	// HitResult 정보를 넘겨준다.
	Context.AddHitResult(HitData->HitResult);

	// GameplayEffect_Damage 생성
	FGameplayEffectSpecHandle	DmgSpec = MakeOutgoingGameplayEffectSpec(
		UGameplayEffect_Damage::StaticClass(), 1.f);
	
	// 이펙트 스펙에 컨텍스트를 등록한다.
	DmgSpec.Data->SetContext(Context);

	// 각각의 ASC로부터 원하는 Attribute를 얻어올 수 있다.
	// SourceASC->GetSet<UBaseAttributeSet>() : ASC가 가지고 있는 AttributeSet을
	// 얻어온다.1
	float Attack = SourceASC->GetSet<UBaseAttributeSet>()->GetAttackPower();
	float Defense = TargetASC->GetSet<UBaseAttributeSet>()->GetDefense();

	float Dmg = Attack - Defense;

	Dmg = FMath::Max(1.f, Dmg);

	// EffectDamage 클래스에서 지정한 SetByCaller에 Dmg 값을 넘겨준다.
	DmgSpec.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(TEXT("Monster.Data.Damage")),
		-Dmg);

	// 타겟 정보를 만들어준다.
	FGameplayAbilityTargetDataHandle	TargetData;

	FGameplayAbilityTargetData_ActorArray* TargetArray =
		new FGameplayAbilityTargetData_ActorArray;

	TargetArray->TargetActorArray.Add(TargetActor);

	TargetData.Add(TargetArray);

	// 타겟 정보를 이용하여 타겟의 이펙트를 호출해 주어야 한다.
	ApplyGameplayEffectSpecToTarget(Handle, ActorInfo, ActivationInfo,
		DmgSpec, TargetData);


	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
