#include "DLTask_ExecuteGameplayAbility.h"

EDialogueResult UDLTask_ExecuteGameplayAbility::ExecuteTask_Implementation(UDialogueResultParam* Param, AActor* Questioner, AActor* Respondent)
{
	if (Questioner == nullptr || Respondent == nullptr)
	{
		return EDialogueResult::End;
	}

	auto* TaskParam = Cast<UDLTaskParam_ExecuteGameplayAbility>(Param);
	if (IsValid(TaskParam) == false)
	{
		return EDialogueResult::End;
	}

	AActor* Instigator;
	AActor* Target;
	if (TaskParam->mTargetType == EExecuteGameplayAbilityTarget::PlayerASC)
	{
		Instigator = Questioner;
		Target = Respondent;
	}
	else
	{
		Instigator = Respondent;
		Target = Questioner;
	}

	/* 전달 방식 */
	switch (TaskParam->mCallMethod)
	{
	case EGameplayAbilityCallMethod::Event:
	{
		FGameplayEventData EventData;
		EventData.EventTag = TaskParam->mEventTag;
		EventData.InstigatorTags = TaskParam->mInstigatorTags;
		EventData.TargetTags = TaskParam->mTargetTags;
		EventData.Instigator = Instigator;
		EventData.Target = Target;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Target, EventData.EventTag, EventData);
		break;
	}
	case EGameplayAbilityCallMethod::Tag:
	{
		UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		if (AbilitySystemComponent != nullptr && IsValidChecked(AbilitySystemComponent))
		{
			AbilitySystemComponent->TryActivateAbilitiesByTag(TaskParam->mTargetTags);
		}
		break;
	}
	case EGameplayAbilityCallMethod::Class:
	{
		UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		if (AbilitySystemComponent != nullptr && IsValidChecked(AbilitySystemComponent))
		{
			AbilitySystemComponent->TryActivateAbilityByClass(TaskParam->mClass);
		}
		break;
	}
	case EGameplayAbilityCallMethod::InputID:
	{
		UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		if (AbilitySystemComponent != nullptr && IsValidChecked(AbilitySystemComponent))
		{
			AbilitySystemComponent->PressInputID(TaskParam->mInputID);
		}
		break;
	}
	}
	return EDialogueResult::End;
}
