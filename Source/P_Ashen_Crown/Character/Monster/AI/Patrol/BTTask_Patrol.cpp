// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrol.h"
#include "../../MonsterBase.h"
#include "../../MonsterAnimInstance.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"

UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName = TEXT("Patrol");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterBase* Monster = OwnerComp.GetAIOwner()->GetPawn<AMonsterBase>();

	if (!IsValid(Monster))
		return EBTNodeResult::Failed;

	UMonsterAnimInstance* AnimInst = Monster->GetMonsterAnim();
	if (!IsValid(AnimInst))
		return EBTNodeResult::Failed;

	UAnimMontage* Montage = AnimInst->GetMontageByType(EMonsterAnim::Walk);

	AcceptableRadius = 0.f;

	UBlackboardComponent* Board = OwnerComp.GetAIOwner()->GetBlackboardComponent();

	if (!IsValid(Board))
		return EBTNodeResult::Failed;

	FVector	Point = Monster->GetPatrolPoint();

	Board->SetValueAsVector(TEXT("PatrolPoint"), Point);

	return Result;
}

void UBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UBTTask_Patrol::OnTaskFinished(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	OwnerComp.GetAIOwner()->StopMovement();

	AMonsterBase* Monster = OwnerComp.GetAIOwner()->GetPawn<AMonsterBase>();
	UMonsterAnimInstance* AnimInst = Monster->GetMonsterAnim();

	if (IsValid(Monster))
	{
		// 태스크 성공일 경우 목적지에 도착해서 종료되었다는 의미이다.
		if (TaskResult == EBTNodeResult::Succeeded)
			Monster->NextPatrolPoint();

		Monster->ChangeAnim(EMonsterAnim::Idle);
	}
}
