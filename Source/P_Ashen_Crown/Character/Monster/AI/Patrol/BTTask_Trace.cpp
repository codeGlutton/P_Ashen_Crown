// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Trace.h"
#include "../../MonsterBase.h"
#include "../../MonsterAnimInstance.h"
#include "AIController.h"

UBTTask_Trace::UBTTask_Trace()
{
	NodeName = TEXT("MonsterTrace");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Trace::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 현재 동작되는 AIController를 얻어온다.
	AAIController* AIController = OwnerComp.GetAIOwner();

	// 블랙보드 컴포넌터를 얻어온다.
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!AIController || !BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(MonsterAIKey::TargetActor));

	if (!Target)
	{
		return EBTNodeResult::Succeeded;
	}

	EPathFollowingRequestResult::Type MoveResult = AIController->MoveToActor(Target);

	// 길찾기에 성공했는지 판단한다.
	if (MoveResult == EPathFollowingRequestResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	AMonsterBase* Monster = Cast<AMonsterBase>(AIController->GetPawn());

	if (!Monster)
	{
		return EBTNodeResult::Failed;
	}

	Monster->ChangeAnim(EMonsterAnim::Run);

	//UE_LOG(SacGame, Warning, TEXT("Trace Execute"));

	return EBTNodeResult::InProgress;
}

void UBTTask_Trace::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	// 현재 동작되는 AIController를 얻어온다.
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController)
		return;

	// 블랙보드에 지정된 타겟이 없어질 경우 태스크 종료.
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

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

	EPathFollowingStatus::Type PathStatus = AIController->GetMoveStatus();

	// 도착인지 실패인지 판단한다.
	if (PathStatus == EPathFollowingStatus::Idle)
	{
		//UE_LOG(SacGame, Warning, TEXT("PathFollowing Idle"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	// 타겟의 위치로 이동시킨다.
	float AttackRange = Monster->GetAbilitySystemComponent()->GetSet<UMonsterAttributeSet>()->GetAttackDistance();

	//UE_LOG(SacGame, Warning, TEXT("AttackRange : %f"), AttackDistance);

	FVector	TargetLocation, MonsterLocation;

	// 타겟의 위치를 얻어온다. 단, 발밑 가운데를 기준으로 위치를 지정할 것이기 때문에 캡슐의
	// 절반 높이만큼을 아래로 내린 위치를 구한다.
	TargetLocation = Target->GetActorLocation();

	// 루트컴포넌트가 캡슐일 경우 절반 높이만큼을 아래로 내려준다.
	UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(Target->GetRootComponent());

	if (Capsule)
	{
		TargetLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();
	}

	// 몬스터 위치를 구한다.
	MonsterLocation = Monster->GetActorLocation();

	Capsule = Cast<UCapsuleComponent>(Monster->GetRootComponent());

	// 몬스터는 루트컴포넌트가 무조건 Capsule로 정해져 있다.
	MonsterLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();

	// 둘 사이의 거리를 구한다.
	float Distance = FVector::Dist(TargetLocation, MonsterLocation);

	// 둘 사이의 거리가 공격할 거리보다 가까울 경우 공격 반경 안에 도착한 것으로 판단한다.
	if (Distance <= AttackRange)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

void UBTTask_Trace::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, 
	uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	//UE_LOG(SacGame, Warning, TEXT("Trace End"));

	AAIController * AIController = OwnerComp.GetAIOwner();

	AIController->StopMovement();
}
