// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DeathAnim.h"
#include "../MonsterBase.h"
#include "../MonsterAnimInstance.h"
#include "AIController.h"


UBTTask_DeathAnim::UBTTask_DeathAnim()
{
	NodeName = TEXT("Death Montage");
}

EBTNodeResult::Type UBTTask_DeathAnim::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(Monster))
		return EBTNodeResult::Failed;

	UAnimMontage* Montage = Monster->GetMonsterAnim()->GetMontageByType(EMonsterAnim::Death);
	if (IsValid(Montage))
	{
		//Monster->GetMonsterAnim()->Montage_Play(Montage);
		Monster->ChangeAnim(EMonsterAnim::Death);
	}

	return EBTNodeResult::Succeeded;
}
