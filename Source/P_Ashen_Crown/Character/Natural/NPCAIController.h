#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "AIController.h"
#include "NPCAIController.generated.h"

/**
 * NPC 용 컨트롤러
 */
UCLASS()
class P_ASHEN_CROWN_API ANPCAIController : public AAIController
{
	GENERATED_BODY()

public:
	ANPCAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
