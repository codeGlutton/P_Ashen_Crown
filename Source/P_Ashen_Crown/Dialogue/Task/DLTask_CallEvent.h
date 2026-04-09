#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/TableRow/DialogueTableRow.h"
#include "DLTask_CallEvent.generated.h"

UCLASS()
class P_ASHEN_CROWN_API UDLTaskParam_CallEvent : public UDialogueResultParam
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "CallEvent")
	FName mEventName;

	// 이미 트리거 되어 있더라도, 강제로 다시 콜 시킬지 여부
	UPROPERTY(EditDefaultsOnly, Category = "CallEvent")
	bool bForcedTrigger;
};

/**
 * 이벤트를 호출하는 Dialogue Task
 */
UCLASS()
class P_ASHEN_CROWN_API UDLTask_CallEvent : public UDialogueResultTask
{
	GENERATED_BODY()
	
public:
	virtual EDialogueResult ExecuteTask_Implementation(UDialogueResultParam* Param, AActor* Questioner, AActor* Respondent) override;
};
