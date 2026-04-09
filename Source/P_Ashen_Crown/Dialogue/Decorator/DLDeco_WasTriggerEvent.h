#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/TableRow/DialogueTableRow.h"
#include "DLDeco_WasTriggerEvent.generated.h"

UCLASS()
class P_ASHEN_CROWN_API UDLDecoParam_WasTriggerEvent : public UDialogueChoiceDecoratorParam
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "WasTriggerEvent")
	FName mEventName;

	UPROPERTY(EditDefaultsOnly, Category = "WasTriggerEvent")
	bool bReverse;
};

/**
 * 이벤트를 호출했었는지를 파악하는 Dialogue Decorator
 */
UCLASS()
class P_ASHEN_CROWN_API UDLDeco_WasTriggerEvent : public UDialogueChoiceDecorator
{
	GENERATED_BODY()
	
public:
	virtual bool CanEnterChoice_Implementation(UDialogueChoiceDecoratorParam* Param, AActor* Questioner, AActor* Respondent) override;
};
