// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_LookAtTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "../MonsterBase.h"
#include "../MonsterAnimInstance.h"

UBTService_LookAtTarget::UBTService_LookAtTarget()
{
	NodeName = TEXT("LookAtTarget");
	//mTargetActor.SelectedKeyName = TEXT("TargetActor");
}

void UBTService_LookAtTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(mTargetActor.SelectedKeyName));
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!IsValid(Target) || !IsValid(ControlledPawn))
		return;

	AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());

	// 회전 잠금 중이면 무시
	/*if (Monster->bLockRotation)
		return;*/

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	bool IsCasting = BlackboardComp->GetValueAsBool(TEXT("IsCastingSkill"));
	if (IsCasting)
	{
		UE_LOG(LogTemp, Warning, TEXT("TickTask: IsCasting = %s"), IsCasting ? TEXT("true") : TEXT("false"));
		return;
	}
	else
	{
		FVector ToTarget = Target->GetActorLocation() - ControlledPawn->GetActorLocation();
		ToTarget.Z = 0.0f;
		ToTarget.Normalize();

		FRotator LookRot = ToTarget.Rotation();
		//FRotator NewRot = FRotator(0.f, LookRot.Yaw, 0.f);
		FRotator NewRot = FMath::RInterpTo(ControlledPawn->GetActorRotation(), FRotator(0.f, LookRot.Yaw, 0.f), DeltaSeconds, 5.f);

		ControlledPawn->SetActorRotation(NewRot);
	}
	
}
