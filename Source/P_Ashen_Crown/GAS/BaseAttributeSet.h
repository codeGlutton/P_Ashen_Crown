// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GASCore.h"
#include "AttributeSet.h"
#include "BaseAttributeSet.generated.h"

/**
 * 플레이어, 몬스터에 기본으로 들어가는 Status을 정의해놓은 AttributeSet
 */
UCLASS()
class P_ASHEN_CROWN_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UBaseAttributeSet();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	MoveSpeed;

public:
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, AttackPower)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Defense)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, HP)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHP)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MoveSpeed)


public:
	/// <summary>
	/// GameplayEffect가 적용되어 Attribute값이 변경된 직후 후처리 로직을 넣을 수 있는 함수
	/// </summary>
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data);
};
