// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "AIController.h"
#include "../MonsterAnimInstance.h"
#include "../MonsterBase.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Monster Attack");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (!IsValid(Monster))
		return EBTNodeResult::Failed;

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("AttackEnd"), false);

	Monster->ChangeAnim(EMonsterAnim::Attack);

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(TEXT("TargetActor")));
	if (!IsValid(Target))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	if (!IsValid(Monster))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//bool AttackEnd = Blackboard->GetValueAsBool(TEXT("AttackEnd"));
	//UE_LOG(LogTemp, Warning, TEXT("AttackEnd = %s"), AttackEnd ? TEXT("true") : TEXT("false"));

	// 애니메이션 종료 감지
	if (Blackboard->GetValueAsBool(TEXT("AttackEnd")))
	{
		Blackboard->SetValueAsBool(TEXT("AttackEnd"), false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}