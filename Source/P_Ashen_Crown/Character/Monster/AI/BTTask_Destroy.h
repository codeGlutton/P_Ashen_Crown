// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Destroy.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UBTTask_Destroy : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Destroy();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
