#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/Component/SightTargetSensingComponent.h"
#include "InteractionSensingComponent.generated.h"

class UInteractableTargetComponent;

/**
 * 상호작용 객체를 탐지하는 컴포넌트
 */
UCLASS()
class P_ASHEN_CROWN_API UInteractionSensingComponent : public USightTargetSensingComponent
{
	GENERATED_BODY()
	
public:
	UInteractionSensingComponent();

public:
	virtual void InitializeComponent() override;

public:
	UInteractableTargetComponent* GetTargetComponent() const;

protected:
	virtual bool IsActorValidCandidate(AActor* DetectedActor) const override;
	virtual AActor* SelectTargetFromCandidates(const TSet<TObjectPtr<AActor>>&CandidateActors) const override;

protected:
	/**
	 * 감지 결과를 받아 상호작용 가능한 타겟 컴포넌트를 동작시키는 함수
	 * @param PreTargetActor 이전 타겟 액터
	 * @param NextTargetActor 이후 타겟 액터
	 */
	UFUNCTION(BlueprintNativeEvent)
	void ChangeTargetComponent(AActor* PreTargetActor, AActor* NextTargetActor);
	virtual void ChangeTargetComponent_Implementation(AActor* PreTargetActor, AActor* NextTargetActor);

protected:
	TObjectPtr<UInteractableTargetComponent> mTargetComponent;
};
