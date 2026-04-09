// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DeathAnim.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UBTTask_DeathAnim : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_DeathAnim();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
