#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/Component/LockOnTargetComponent.h"
#include "LockOnTargetActorComponent.generated.h"

/**
 * 해당 액터에 락온할 수 있도록 해주는 컴포넌트
 */
UCLASS()
class P_ASHEN_CROWN_API ULockOnTargetActorComponent : public ULockOnTargetComponent
{
	GENERATED_BODY()

public:
	virtual void OpenLockOnView(AActor* Instigator, APlayerController* PlayerController) override;
	virtual void CloseLockOnView() override;
};
