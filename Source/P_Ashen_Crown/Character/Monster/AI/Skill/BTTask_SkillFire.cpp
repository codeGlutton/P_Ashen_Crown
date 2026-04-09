// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SkillFire.h"
#include "AIController.h"
#include "../../MonsterAnimInstance.h"
#include "../../BossMonster/BossMonster.h"


UBTTask_SkillFire::UBTTask_SkillFire()
{
	NodeName = TEXT("Boss SkillFire");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_SkillFire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return EBTNodeResult::Failed;

	// 블랙보드 컴포넌트를 얻어온다.
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
		return EBTNodeResult::Failed;

	//UE_LOG(LogTemp, Warning, TEXT("Tick Blackboard Ptr: %p"), BlackboardComp);

	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(MonsterAIKey::TargetActor));
	if (!Target)
		return EBTNodeResult::Succeeded;

	ABossMonster* Monster = Cast<ABossMonster>(AIController->GetPawn());
	if (!Monster)
		return EBTNodeResult::Failed;

	// 쿨타임 중 Task 실패 -> 애니메이션 재생 차단
	if (Monster->GetAbilitySystemComponent()->HasMatchingGameplayTag(
		FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.ProjectileSkill"))))
	{
		//UE_LOG(LogTemp, Warning, TEXT("[SkillFire] 쿨타임 중 Task 종료"));
		return EBTNodeResult::Failed;
	}

	float Distance = FVector::Dist(Monster->GetActorLocation(), Target->GetActorLocation());

	// 거리 400 넘을 때만 발동
	if (Distance <= 400.f)
	{
		return EBTNodeResult::Failed;
	}

	// 몽타주 재생
	Monster->GetMonsterAnim()->PlayMontageByType(EMonsterAnim::Skill1);

	// 공격용 타겟 설정
	BlackboardComp->SetValueAsObject(MonsterAIKey::AttackTarget, Target);

	bool AttackEnd = BlackboardComp->GetValueAsBool(MonsterAIKey::AttackEnd);
	//UE_LOG(LogTemp, Warning, TEXT("TickTask: AttackEnd = %s"), AttackEnd ? TEXT("true") : TEXT("false"));

	return EBTNodeResult::InProgress;
}

void UBTTask_SkillFire::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//UE_LOG(LogTemp, Warning, TEXT("SkillFire TickTask 진입"));

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

	ABossMonster* Monster = Cast<ABossMonster>(AIController->GetPawn());
	if (!Monster)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	if (Monster->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.MagicFloorSkill"))))
	{
		// 쿨타임 중이면 Task 실패로 바로 종료
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	bool AttackEnd = BlackboardComp->GetValueAsBool(MonsterAIKey::AttackEnd);
	//UE_LOG(LogTemp, Warning, TEXT("AttackEnd = %s"), AttackEnd ? TEXT("true") : TEXT("false"));

	if (AttackEnd)
	{
		// 공격 종료 후 처리
		BlackboardComp->SetValueAsBool(MonsterAIKey::AttackEnd, false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}

void UBTTask_SkillFire::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	// 공격용 타겟 설정.
	BlackboardComp->SetValueAsObject(MonsterAIKey::AttackTarget, nullptr);

	
}

