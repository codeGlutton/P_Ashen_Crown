// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackSignal.h"
#include "AIController.h"
#include "../MonsterAnimInstance.h"
#include "../MonsterAIController.h"
#include "../MonsterBase.h"

UBTTask_AttackSignal::UBTTask_AttackSignal()
{
	NodeName = TEXT("Attack Signal");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_AttackSignal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(Monster))
		return EBTNodeResult::Failed;

	Monster->ChangeAnim(EMonsterAnim::Signal1);

	/*UMonsterAnimInstance* AnimInst = Monster->GetMonsterAnim();
	if (!IsValid(AnimInst))
		return EBTNodeResult::Failed;

	UAnimMontage* Montage = AnimInst->GetMontageByType(EMonsterAnim::Signal1);
	if (!IsValid(Montage))
		return EBTNodeResult::Failed;

	AnimInst->Montage_Play(Montage);*/
	//bIsMontagePlaying = true;

	/*if (!IsValid(Montage))
	{
		UE_LOG(LogTemp, Error, TEXT("[AttackSignal] 몽타주가 유효하지 않음: Signal1"));
		return EBTNodeResult::Failed;
	}*/

	AMonsterAIController* AIController = Cast<AMonsterAIController>(Monster->GetController());
	if (IsValid(AIController))
	{
		AIController->StopMovement();
	}

	/*if (AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")))
		UC_Util::Print("Before TickTask : TargetActor is valid!", FColor::Red, 10.f);
	else UC_Util::Print("Before TickTask : TargetActor not valid!", FColor::Red, 10.f);*/

	return EBTNodeResult::InProgress;
}

void UBTTask_AttackSignal::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (!IsValid(Monster))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UAnimInstance* AnimInst = Monster->GetMesh()->GetAnimInstance();
	UMonsterAnimInstance* MonsterAnimInst = Cast<UMonsterAnimInstance>(AnimInst);
	if (!IsValid(MonsterAnimInst))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	/*if (MonsterAnimInst->IsMontageCurrentlyPlaying(EMonsterAnim::Signal1)) 
		return;*/

	if (!AnimInst->Montage_IsPlaying(AnimInst->GetCurrentActiveMontage()))
	{
		AMonsterAIController* AIController = Cast<AMonsterAIController>(Monster->GetController());
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
