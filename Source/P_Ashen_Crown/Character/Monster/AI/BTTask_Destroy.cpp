// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Destroy.h"
#include "AIController.h"

UBTTask_Destroy::UBTTask_Destroy()
{
	NodeName = TEXT("Destroy");
}

EBTNodeResult::Type UBTTask_Destroy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	if (Pawn)
	{
		Pawn->Destroy();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
