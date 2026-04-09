// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Attack();


private:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComp;

	bool bAttacking = false;
public:
	// 테스트 실행 시 호출
	// @param OwnerComp  : 현재 실행 중인 BehaviorTree
	// @param NodeMemory : 노드 메모리 포인터
	// @return			 : 태스크 실행 결과(InProgress, Succeeded, Failed)
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	//virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult);

};

