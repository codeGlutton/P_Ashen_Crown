#include "DLTask_CallEvent.h"
#include "P_Ashen_Crown/Singleton/Subsystem/GameEventSubsystem.h"

EDialogueResult UDLTask_CallEvent::ExecuteTask_Implementation(UDialogueResultParam* Param, AActor* Questioner, AActor* Respondent)
{
	if (Questioner == nullptr || Respondent == nullptr)
	{
		return EDialogueResult::End;
	}

	auto* TaskParam = Cast<UDLTaskParam_CallEvent>(Param);
	if (IsValid(TaskParam) == false)
	{
		return EDialogueResult::End;
	}

	auto* GameEventSubsystem = Respondent->GetWorld()->GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
	if (IsValid(GameEventSubsystem) == false)
	{
		return EDialogueResult::End;
	}

	if (TaskParam->bForcedTrigger == true && GameEventSubsystem->WasTriggerEvent(TaskParam->mEventName) == true)
	{
		GameEventSubsystem->ResetEvent(TaskParam->mEventName);
	}
	GameEventSubsystem->CallEvent(TaskParam->mEventName, Questioner);
	return EDialogueResult::End;
}
