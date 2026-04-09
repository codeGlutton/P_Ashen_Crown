// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackSignal.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UBTTask_AttackSignal : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_AttackSignal();


protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
