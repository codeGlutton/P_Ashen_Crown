#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ResetBBEntries.generated.h"

/**
 * 지정된 블랙 보드 엔트리를 리셋하는 Task
 */
UCLASS()
class P_ASHEN_CROWN_API UBTTask_ResetBBEntries : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "ResetBBEntries", meta = (DisplayName = "ResetEntries"))
	TArray<FBlackboardKeySelector> mResetEntries;
};
