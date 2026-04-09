#include "NPCAIController.h"
#include "P_Ashen_Crown/Character/Natural/NPC.h"
#include "BehaviorTree/BlackboardComponent.h"

ANPCAIController::ANPCAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void ANPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	auto* NPCPawn = Cast<ANPC>(InPawn);
	if (IsValid(NPCPawn) == true)
	{
		UBlackboardComponent* OwnBlackBoradComp;
		if (UseBlackboard(NPCPawn->GetBlackBoardData(), OwnBlackBoradComp) == true)
		{
			RunBehaviorTree(NPCPawn->GetBehaviorTree());
		}
	}
}

