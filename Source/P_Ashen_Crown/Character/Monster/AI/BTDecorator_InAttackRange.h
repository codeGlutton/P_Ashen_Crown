// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_InAttackRange.generated.h"

/**
 * 타겟이 지정된 공격 거리(mAttackDistance) 이내에 있는지 확인합니다.
 * 조건이 참일 경우 하위 노드를 실행합니다.
 */
UCLASS()
class P_ASHEN_CROWN_API UBTDecorator_InAttackRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_InAttackRange();

protected:
	UPROPERTY(EditAnywhere, Category = "InAttackRange")
	FBlackboardKeySelector mTargetActor;

	UPROPERTY(EditAnywhere, Category = "InAttackRange")
	float mAttackDistance = 400.f;

protected:
	/**
	 * 실제 데코레이터 조건 판단을 수행하는 함수입니다.
	 * @param OwnerComp		비헤이비어 트리 컴포넌트
	 * @param NodeMemory	데코레이터 노드 메모리
	 * @return				true일 경우 조건 통과
	 */
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
