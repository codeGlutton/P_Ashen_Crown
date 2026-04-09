// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayEffect_Damage.h"
#include "../BaseAttributeSet.h"
#include "../AttributeSet/MonsterAttributeSet.h"

UGameplayEffect_Damage::UGameplayEffect_Damage()
{
	// 효과 타입을 지정한다.
	// Instant : 즉시 적용.(데미지 같은 곳에서 주로 사용)
	// HasDuration : 일정 시간동안 지속.(버프, 디버프 등)
	// Infinite : 무한으로 지속.(패시브같은 것들.)
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// HasDuration일 경우 지속시간을 지정한다.
	//DurationMagnitude = FScalableFloat(2.f);

	// FGameplayModifierInfo : 어떻게 연산할 지 등의 속성을 지정한다.
	FGameplayModifierInfo	Modifier;

	// 만약 데미지가 외부에서 계산되어 적용이 되어야 하는 경우라면
	// SetByCaller라는 것을 활용하여 외부에서 데미지를 지정할 수 있도록 처리하낟.
	FSetByCallerFloat	Caller;
	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Monster.Data.Damage"));

	// HP 속성을 변경시키겠다는 의미이다.
	Modifier.Attribute = UBaseAttributeSet::GetHPAttribute();

	// 연산 방법을 지정한다.
	Modifier.ModifierOp = EGameplayModOp::Additive;

	// 위에서 구해준 SetByCaller를 지정한다.
	Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	Modifiers.Add(Modifier);

	// 지정한 태그로 되어 있는 Cue를 동작시키라는 의미이다.
	FGameplayEffectCue	Cue;
	//Cue.GameplayCueTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.Cue.Attack")));

	//GameplayCues.Add(Cue);

	// GameplayCues 동작시킬 Cue를 지정한다.



	// GameplayEffect가 적용되면 부여되는 태그 지정.
	//InheritableOwnedTagsContainer.AddTag(AttackTag);

	// ExecutionCalculation 클래스들을 연결. 계산을 할 경우 사용한다.
	// Modifier 대신 복잡한 로직을 처리할 수 있다.
	//Executions.Add(FGameplayEffectExecutionDefinition(UDamageExecutionCalculation::StaticClass()));

	// InheritableGameplayEffectTags : 이 GameplayEffect 자체가 가지는 태그. 효과 자체에 태그를 부여한다.

	// ApplicationTagRequirements : 이 효과가 적용되기 위해 대상이 가지고 있어야 하는 태그 조건을 지정.

	// OngoingTagRequirements : GE가 적용된 후에도 이 태그 조건이 충족되지 않으면 효과가 제거됨.

	// RemovalTagRequirements : 이 조건이 충족되면 효과가 강제 제거됨.

	// StackLimitCount : 중첩 관련. 동일 효과가 몇개까지 중첩될지.

	// ChanceToApplytoTarget : 적용 확률을 지정한다. FScalableFloat이다.

	// OverflowEffects : 스택이 최대일 때 추가로 적용할 GE
	// 중첩이 가득 차면 데미지를 추가로 부여하는 등의 처리를 한다.

	// ConditionalGameplayEffects : 특정 태그 조건이 맞을 때만 함께 적용되는 부가 GE

	// GameplayEffectApplicationRequirements : C++로 효과 적용 조건을 직접 구현할 수 있는 클래스 연결. 체력 50% 이상일 때만 적용 등의 효과를 구현할 때 좋다.
}
