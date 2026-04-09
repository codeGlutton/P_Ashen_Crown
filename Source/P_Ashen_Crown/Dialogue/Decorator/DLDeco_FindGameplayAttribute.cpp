#include "DLDeco_FindGameplayAttribute.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"
#include "GameplayAbilities/Public/AbilitySystemBlueprintLibrary.h"

bool UDLDeco_FindGameplayAttribute::CanEnterChoice_Implementation(UDialogueChoiceDecoratorParam* Param, AActor* Questioner, AActor* Respondent)
{
	if (Questioner == nullptr || Respondent == nullptr)
	{
		return false;
	}

	auto* DecoParam = Cast<UDLDecoParam_FindGameplayAttribute>(Param);
	if (IsValid(DecoParam) == false)
	{
		return false;
	}

	UAbilitySystemComponent* TargetASC = nullptr;
	switch (DecoParam->mTargetAttribute)
	{
	case EGameplayAttributeFinderTarget::Speaker:
		TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Questioner);
		break;
	case EGameplayAttributeFinderTarget::Player:
		TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Respondent);
		break;
	}
	bool bIsFound = false;
	float TargetValue = TargetASC->GetGameplayAttributeValue(DecoParam->mAtrribute, bIsFound);

	switch (DecoParam->mValueFilter)
	{
	case EGameplayAttributeValueComparisonType::Equal:
		return DecoParam->mCompareValue == TargetValue;
	case EGameplayAttributeValueComparisonType::NotEqual:
		return DecoParam->mCompareValue != TargetValue;
	case EGameplayAttributeValueComparisonType::Greater:
		return DecoParam->mCompareValue > TargetValue;
	case EGameplayAttributeValueComparisonType::Less:
		return DecoParam->mCompareValue < TargetValue;
	case EGameplayAttributeValueComparisonType::GreaterOrEqual:
		return DecoParam->mCompareValue >= TargetValue;
	case EGameplayAttributeValueComparisonType::LessOrEqual:
		return DecoParam->mCompareValue <= TargetValue;
	case EGameplayAttributeValueComparisonType::None:
		return true;
	}
	return false;
}
