#include "BTTask_ResetBBEntries.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"

EBTNodeResult::Type UBTTask_ResetBBEntries::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (IsValid(BlackBoardComp) == false)
	{
		return EBTNodeResult::Failed;
	}

	for (auto& Entry : mResetEntries)
	{
		BlackBoardComp->ClearValue(Entry.SelectedKeyName);
	}
	return EBTNodeResult::Succeeded;
}
