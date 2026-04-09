#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CalDistToTarget.generated.h"

/**
 * 타겟과의 거리를 계속해서 검사하는 서비스 노드
 */
UCLASS()
class P_ASHEN_CROWN_API UBTService_CalDistToTarget : public UBTService
{
	GENERATED_BODY()
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "CalDistToTarget")
	FBlackboardKeySelector mTargetActor;

	UPROPERTY(EditAnywhere, Category = "CalDistToTarget")
	FBlackboardKeySelector mResultDist;
};
