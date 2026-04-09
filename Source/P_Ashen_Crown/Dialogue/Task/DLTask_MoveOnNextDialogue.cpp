#include "DLTask_MoveOnNextDialogue.h"
#include "P_Ashen_Crown/Component/DialogueComponent.h"

EDialogueResult UDLTask_MoveOnNextDialogue::ExecuteTask_Implementation(UDialogueResultParam* Param, AActor* Questioner, AActor* Respondent)
{
	if (Questioner == nullptr || Respondent == nullptr)
	{
		return EDialogueResult::End;
	}

	auto* TaskParam = Cast<UDLTaskParam_MoveOnNextDialogue>(Param);
	if (IsValid(TaskParam) == false)
	{
		return EDialogueResult::End;
	}

	auto* DialogueComp = Respondent->FindComponentByClass<UDialogueComponent>();
	if (IsValid(DialogueComp) == false)
	{
		return EDialogueResult::End;
	}
	DialogueComp->ContinueDialogue(TaskParam->mNextDialogueKey);
	return EDialogueResult::Continue;
}
