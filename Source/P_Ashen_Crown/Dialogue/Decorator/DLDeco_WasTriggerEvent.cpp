#include "DLDeco_WasTriggerEvent.h"
#include "P_Ashen_Crown/Singleton/Subsystem/GameEventSubsystem.h"

bool UDLDeco_WasTriggerEvent::CanEnterChoice_Implementation(UDialogueChoiceDecoratorParam* Param, AActor* Questioner, AActor* Respondent)
{
	if (Questioner == nullptr || Respondent == nullptr)
	{
		return false;
	}

	auto* DecoParam = Cast<UDLDecoParam_WasTriggerEvent>(Param);
	if (IsValid(DecoParam) == false)
	{
		return false;
	}

	auto* GameEventSubsystem = Respondent->GetWorld()->GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
	if (IsValid(GameEventSubsystem) == false)
	{
		return false;
	}
	
	return GameEventSubsystem->WasTriggerEvent(DecoParam->mEventName) != DecoParam->bReverse;
}
