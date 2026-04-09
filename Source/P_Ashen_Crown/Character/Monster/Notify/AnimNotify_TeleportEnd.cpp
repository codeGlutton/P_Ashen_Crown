// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_TeleportEnd.h"
#include "../MonsterBase.h"
#include "../BossMonster/BossLich.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UAnimNotify_TeleportEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UE_LOG(LogTemp, Warning, TEXT(">>> TeleportEnd Notify 진입"));

	AMonsterBase* Monster = Cast<AMonsterBase>(MeshComp->GetOwner());
	if (!IsValid(Monster))
		return;

	AAIController* AIController = Cast<AAIController>(Monster->GetController());
	if (!IsValid(AIController))
		return;

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (IsValid(Blackboard))
	{
		Blackboard->SetValueAsBool(MonsterAIKey::TeleportEnd, true);
		Monster->bLockRotation = true;
		UE_LOG(LogTemp, Warning, TEXT("[NotifyClass] TeleportEnd = true 설정 완료"));
	}

	if (IsValid(Blackboard))
	{
		// 종료 처리
		Blackboard->SetValueAsBool(TEXT("IsCastingSkill"), false);
		UE_LOG(LogTemp, Warning, TEXT("IsCastingSkill = true 설정됨"));
	}
}