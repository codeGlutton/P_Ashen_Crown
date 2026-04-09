#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "Components/ActorComponent.h"
#include "CameraTargetingBlendComponent.generated.h"

/**
 * 타겟팅 대상에 카메라 회전을 계속 보간해주면서 따라가는 컴포넌트
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P_ASHEN_CROWN_API UCameraTargetingBlendComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCameraTargetingBlendComponent();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
public:
	void SetTargetActor(AActor* TargetActor);

protected:
	TObjectPtr<AActor> mTargetActor;
};
