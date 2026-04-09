#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/TableRow/DialogueTableRow.h"
#include "DLTask_MoveOnNextDialogue.generated.h"

UCLASS()
class P_ASHEN_CROWN_API UDLTaskParam_MoveOnNextDialogue : public UDialogueResultParam
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "MoveOnNextDialogue")
	FName mNextDialogueKey;
};

/**
 * 다음 대화문으로 넘어가는 Dialogue Task
 */
UCLASS()
class P_ASHEN_CROWN_API UDLTask_MoveOnNextDialogue : public UDialogueResultTask
{
	GENERATED_BODY()

public:
	virtual EDialogueResult ExecuteTask_Implementation(UDialogueResultParam* Param, AActor* Questioner, AActor* Respondent) override;
};
