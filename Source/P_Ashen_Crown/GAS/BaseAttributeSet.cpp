// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttributeSet.h"

UBaseAttributeSet::UBaseAttributeSet()
{
}

void UBaseAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//if (Data.EvaluatedData.Attribute == )
}