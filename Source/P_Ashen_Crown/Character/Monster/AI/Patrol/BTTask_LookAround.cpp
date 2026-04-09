// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LookAround.h"
#include "../../MonsterBase.h"
#include "../../MonsterAnimInstance.h"
#include "AIController.h"

UBTTask_LookAround::UBTTask_LookAround()
{
	NodeName = TEXT("LookAround");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_LookAround::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterBase* Monster = OwnerComp.GetAIOwner()->GetPawn<AMonsterBase>();

	UMonsterAnimInstance* AnimInst = Monster->GetMonsterAnim();
	if (!IsValid(AnimInst))
		return EBTNodeResult::Failed;


	if (IsValid(Monster))
	{
		UAnimMontage* Montage = AnimInst->GetMontageByType(EMonsterAnim::Idle);
		if (!IsValid(Montage))
			return EBTNodeResult::Failed;

		AnimInst->Montage_Play(Montage);

		UBlackboardComponent* Board = OwnerComp.GetAIOwner()->GetBlackboardComponent();

		// 기다리기 시작해야 하기 때문에 누적 시간을 0으로 초기화한다.
		if (IsValid(Board))
			Board->SetValueAsFloat(TEXT("AccLookAroundTime"), 0.f);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_LookAround::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Board = OwnerComp.GetAIOwner()->GetBlackboardComponent();

	if (IsValid(Board))
	{
		float	AccTime = Board->GetValueAsFloat(TEXT("AccLookAroundTime"));

		AccTime += DeltaSeconds;

		if (AccTime >= Board->GetValueAsFloat(BlackboardKey.SelectedKeyName))
		{
			AccTime = 0.f;

			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}

		Board->SetValueAsFloat(TEXT("AccLookAroundTime"), AccTime);
	}
}

void UBTTask_LookAround::OnTaskFinished(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

}
