// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GASCore.h"
#include "../BaseAttributeSet.h"
#include "PlayerAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UPlayerAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()

public:
	UPlayerAttributeSet();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData StaminaMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData Weight;

public:
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Stamina)
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, StaminaMax)
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MP)
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Weight)

protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
