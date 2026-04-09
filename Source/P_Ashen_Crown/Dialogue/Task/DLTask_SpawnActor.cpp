#include "DLTask_SpawnActor.h"

EDialogueResult UDLTask_SpawnActor::ExecuteTask_Implementation(UDialogueResultParam* Param, AActor* Questioner, AActor* Respondent)
{
	if (Questioner == nullptr || Respondent == nullptr)
	{
		return EDialogueResult::End;
	}

	auto* TaskParam = Cast<UDLTaskParam_SpawnActor>(Param);
	if (IsValid(TaskParam) == false)
	{
		return EDialogueResult::End;
	}

	Respondent->GetWorld()->SpawnActor<AActor>(TaskParam->mTargetActorClass, TaskParam->mSpawnTransform);
	return EDialogueResult::End;
}
