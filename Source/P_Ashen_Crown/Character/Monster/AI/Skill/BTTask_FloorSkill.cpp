// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FloorSkill.h"
#include "AIController.h"
#include "../../MonsterAnimInstance.h"
#include "../../BossMonster/BossMonster.h"
#include "../../MonsterBase.h"

UBTTask_FloorSkill::UBTTask_FloorSkill()
{
	NodeName = TEXT("Boss FloorSkill");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_FloorSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 현재 동작되는 AIController를 얻어온다.
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	// 블랙보드 컴포넌트를 얻어온다.
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
		return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(MonsterAIKey::TargetActor));
	if (!Target)
		return EBTNodeResult::Succeeded;

	AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(Monster)) 
		return EBTNodeResult::Failed;

	ABossMonster* Boss = Cast<ABossMonster>(Monster);
	if (!Boss)
		return EBTNodeResult::Failed;

	if (Monster->GetAbilitySystemComponent()->HasMatchingGameplayTag(
		FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.MagicFloorSkill"))))
	{
		//UE_LOG(LogTemp, Warning, TEXT("장판 스킬 쿨타임 중"));
		return EBTNodeResult::Failed;
	}

	float Distance = FVector::Dist(Monster->GetActorLocation(), Target->GetActorLocation());

	if (Distance > 400.f)
	{
		return EBTNodeResult::Failed;
	}

	// 몽타주 재생
	Monster->GetMonsterAnim()->PlayMontageByType(EMonsterAnim::Skill2);

	// 공격용 타겟 설정
	BlackboardComp->SetValueAsObject(MonsterAIKey::AttackTarget, Target);

	// 스킬 종료 초기화
	//BlackboardComp->SetValueAsBool(MonsterAIKey::SkillEnd, false);
	//bool AttackEnd = BlackboardComp->GetValueAsBool(MonsterAIKey::AttackEnd);
	bool SkillEnd = BlackboardComp->GetValueAsBool(MonsterAIKey::SkillEnd);
	UE_LOG(LogTemp, Warning, TEXT("[TickTask] SkillEnd = %s"), SkillEnd ? TEXT("true") : TEXT("false"));

	return EBTNodeResult::InProgress;
}

void UBTTask_FloorSkill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!AIController || !Blackboard)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(MonsterAIKey::TargetActor));
	if (!Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	ABossMonster* Monster = Cast<ABossMonster>(AIController->GetPawn());
	if (!IsValid(Monster))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("BTTask_FloorSkill Tick 중"));
	// 공격이 끝났을때 거리를 체크한다.
	//bool AttackEnd = Blackboard->GetValueAsBool(MonsterAIKey::AttackEnd);
	//UE_LOG(LogTemp, Warning, TEXT("AttackEnd = %s"), AttackEnd ? TEXT("true") : TEXT("false"));

	//if (AttackEnd)
	//{
	//	// 공격 종료 후 처리
	//	Blackboard->SetValueAsBool(MonsterAIKey::AttackEnd, false);
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//}


	bool SkillEnd = Blackboard->GetValueAsBool(MonsterAIKey::SkillEnd);
	if (SkillEnd)
	{
		Blackboard->SetValueAsBool(MonsterAIKey::SkillEnd, false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_FloorSkill::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	BlackboardComp->SetValueAsObject(MonsterAIKey::AttackTarget, nullptr);
}