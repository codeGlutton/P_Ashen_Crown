#include "DialogueTableRow.h"

/************************
FDialogueChoiceDecoratorContext
*************************/

bool FDialogueChoiceDecoratorContext::CanEnterChoice(AActor* Questioner, AActor* Respondent)
{
	if (mDecorator == nullptr)
	{
		return false;
	}

	UDialogueChoiceDecoratorParam* ParamObj = nullptr;
	if (mParam.ToSoftObjectPath().IsNull() == false)
	{
		ParamObj = mParam.LoadSynchronous();
	}
	return mDecorator.GetDefaultObject()->CanEnterChoice(ParamObj, Questioner, Respondent);
}

/************************
FDialogueResultTaskContext
*************************/

EDialogueResult FDialogueResultTaskContext::ExecuteTask(AActor* Questioner, AActor* Respondent) const
{
	if (mTask == nullptr)
	{
		return EDialogueResult::End;
	}

	UDialogueResultParam* ParamObj = nullptr;
	if (mParam.ToSoftObjectPath().IsNull() == false)
	{
		ParamObj = mParam.LoadSynchronous();
	}
	return mTask.GetDefaultObject()->ExecuteTask(ParamObj, Questioner, Respondent);
}

/************************
	 UDialogueMidTask
*************************/

void UDialogueMidTask::ExecuteMidTask(UDialogueResultParam* Param, AActor* Questioner, AActor* Respondent)
{
	ExecuteTask(Param, Questioner, Respondent);
}

/************************
 FDialogueMidTaskContext
*************************/

void FDialogueMidTaskContext::ExecuteTask(AActor* Questioner, AActor* Respondent) const
{
	if (mMidTask == nullptr)
	{
		return;
	}

	UDialogueResultParam* ParamObj = nullptr;
	if (mParam.ToSoftObjectPath().IsNull() == false)
	{
		ParamObj = mParam.LoadSynchronous();
	}
	mMidTask.GetDefaultObject()->ExecuteMidTask(ParamObj, Questioner, Respondent);
}
