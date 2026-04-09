// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Teleport.h"
#include "../../MonsterBase.h"
#include "../../MonsterAnimInstance.h"
#include "AIController.h"
//#include "../../../GAS/GameplayEffect/GE_TeleportCooldown.h"

UBTTask_Teleport::UBTTask_Teleport()
{
	NodeName = TEXT("Teleport");
	bNotifyTick = true;

}

EBTNodeResult::Type UBTTask_Teleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(Monster))
		return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(MonsterAIKey::TargetActor));
	if (!Target)
		return EBTNodeResult::Succeeded;

	if (Monster->GetAbilitySystemComponent()->HasMatchingGameplayTag(
		FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.TeleportSkill"))))
	{
		//UE_LOG(LogTemp, Warning, TEXT("텔레포트 쿨타임 중 - 사용 불가"));
		return EBTNodeResult::Failed;
	}

	float Distance = FVector::Dist(Monster->GetActorLocation(), Target->GetActorLocation());
	bool bFloorSkillOnCooldown = Monster->GetAbilitySystemComponent()->HasMatchingGameplayTag(
		FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.MagicFloorSkill")));

	if (Distance > 400.f || !bFloorSkillOnCooldown)
		return EBTNodeResult::Failed;

	//bool AttackEnd = BlackboardComp->GetValueAsBool(MonsterAIKey::TeleportEnd);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(MonsterAIKey::TeleportEnd, false);


	Monster->GetMonsterAnim()->PlayMontageByType(EMonsterAnim::Skill3);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsCastingSkill"),true);
	//Monster->bLockRotation = false;

	return EBTNodeResult::InProgress;
}

void UBTTask_Teleport::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// 현재 동작되는 AIController를 얻어온다.
	AAIController* AIController = OwnerComp.GetAIOwner();

	// 블랙보드에 지정된 타겟이 없어질 경우 태스크 종료.
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(MonsterAIKey::TargetActor));
	if (!Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	AMonsterBase* Monster = Cast<AMonsterBase>(AIController->GetPawn());
	if (!Monster)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	//if (Monster->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.TeleportSkill"))))
	//{
	//	// 쿨타임 중이면 Task 실패로 바로 종료
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	//	return;
	//}

	//bool TeleportEnd = BlackboardComp->GetValueAsBool(MonsterAIKey::TeleportEnd);
	//UE_LOG(LogTemp, Warning, TEXT("TeleportEnd = %s"), TeleportEnd ? TEXT("true") : TEXT("false"));

	if (BlackboardComp->GetValueAsBool(MonsterAIKey::TeleportEnd))
	{
		// 종료 처리
		BlackboardComp->SetValueAsBool(MonsterAIKey::TeleportEnd, false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}