// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_InAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTDecorator_InAttackRange::UBTDecorator_InAttackRange()
{
	NodeName = TEXT("IsTargetInAttackRange");
}

bool UBTDecorator_InAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	const APawn* SelfPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!IsValid(SelfPawn) || !BlackboardComp)
		return false;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(mTargetActor.SelectedKeyName));
	if (!IsValid(TargetActor))
		return false;

	const float Distance = FVector::Dist(SelfPawn->GetActorLocation(), TargetActor->GetActorLocation());
	return Distance <= mAttackDistance;
}
