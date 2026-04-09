// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Character/Monster/MonsterInfo.h"
#include "../BaseAttributeSet.h"
#include "AttributeSet.h"
#include "MonsterAttributeSet.generated.h"

/**
 * 몬스터의 정보를 세팅한다.
 * string 타입은 저장이 안되기 때문에 이름은 따로 저장한다.
 * 몬스터 이름 일반 변수로 따로 저장한다.
 */
UCLASS()
class P_ASHEN_CROWN_API UMonsterAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()
	
public:
	UMonsterAttributeSet();


protected:
	// 마나
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	MP;

	// 공격 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterAttribute")
	FGameplayAttributeData	AttackDistance;

	// 보스의 마법 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterAttribute")
	FGameplayAttributeData	SpellDamage;

	// 소환 가능한 스켈레톤 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterAttribute")
	FGameplayAttributeData SummonCount;

	// 보호막 여부 (마법 보호막이 켜져 있는 상태 - 켜져 있으면 1.f, 꺼져 있으면 0.f)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterAttribute")
	FGameplayAttributeData IsShielded;

public:
	ATTRIBUTE_ACCESSORS(UMonsterAttributeSet, MP)
	ATTRIBUTE_ACCESSORS(UMonsterAttributeSet, AttackDistance)
	ATTRIBUTE_ACCESSORS(UMonsterAttributeSet, SpellDamage)

	ATTRIBUTE_ACCESSORS(UMonsterAttributeSet, SummonCount)
	ATTRIBUTE_ACCESSORS(UMonsterAttributeSet, IsShielded)

public:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

};
