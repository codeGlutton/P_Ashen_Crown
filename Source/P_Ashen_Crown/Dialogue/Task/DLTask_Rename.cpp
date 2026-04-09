#include "DLTask_Rename.h"
#include "P_Ashen_Crown/Character/BaseCharacter.h"

EDialogueResult UDLTask_Rename::ExecuteTask_Implementation(UDialogueResultParam* Param, AActor* Questioner, AActor* Respondent)
{
	if (Questioner == nullptr || Respondent == nullptr)
	{
		return EDialogueResult::End;
	}

	auto* TaskParam = Cast<UDLTaskParam_Rename>(Param);
	if (IsValid(TaskParam) == false)
	{
		return EDialogueResult::End;
	}

	AActor* TargetActor = nullptr;
	if (TaskParam->mRenameTarget == ERenameTarget::PlayerName)
	{
		TargetActor = Respondent;
	}
	else
	{
		TargetActor = Questioner;
	}

	auto* TargetCharacter = Cast<ABaseCharacter>(TargetActor);
	if (IsValid(TargetCharacter) == true)
	{
		TargetCharacter->SetDisplayName(TaskParam->mDisplayName);
	}
	return EDialogueResult::End;
}
