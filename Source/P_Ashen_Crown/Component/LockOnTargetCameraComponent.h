#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/Component/LockOnTargetComponent.h"
#include "LockOnTargetCameraComponent.generated.h"

/**
 * 타 카메라에 락온할 수 있도록 (동적으로 카메라를 변경할 수 있도록) 해주는 컴포넌트
 */
UCLASS()
class P_ASHEN_CROWN_API ULockOnTargetCameraComponent : public ULockOnTargetComponent
{
	GENERATED_BODY()
	
public:
	ULockOnTargetCameraComponent();

public:
	virtual void OpenLockOnView(AActor* Instigator, APlayerController* PlayerController) override;
	virtual void CloseLockOnView() override;

public:
	void SetBlendTime(float BlendTime);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "LockOnTargetCameraComponent", meta = (DisplayName = "블렌드 타임"))
	float mBlendTime;
};
