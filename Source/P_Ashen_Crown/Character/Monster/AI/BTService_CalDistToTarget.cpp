#include "BTService_CalDistToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void UBTService_CalDistToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	AActor* SelfMonster = OwnerComp.GetAIOwner()->GetPawn();
	if (IsValid(SelfMonster) == false)
	{
		return;
	}

	FVector SelfLocation = SelfMonster->GetActorLocation();
	UObject* TargetObject = BlackBoardComp->GetValueAsObject(mTargetActor.SelectedKeyName);
	if (IsValid(TargetObject) == true)
	{
		AActor* TargetActor = Cast<AActor>(TargetObject);
		//  거리 계산
		double Dist = FVector::Dist(SelfLocation, TargetActor->GetActorLocation());

		// 거리 float 저장
		BlackBoardComp->SetValueAsFloat(mResultDist.SelectedKeyName, Dist);
	}
}
