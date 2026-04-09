#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/Component/SightTargetSensingComponent.h"
#include "LockOnSensingComponent.generated.h"

class ULockOnTargetComponent;

/************************
	FPendingLockOnInput
*************************/

/**
 * 예약된 락온 인풋 정보
 */
USTRUCT(BlueprintType)
struct FPendingLockOnInput
{
	GENERATED_BODY()

public:
	void SetLockOn(bool bWillBeLockOn);
	bool IsLockOn() const;

	void SetTargetSwitchOffset(int32 TargetSwitchOffset);
	int32 GetTargetSwitchOffset() const;

	void SetManualLockOnTarget(AActor* ManualLockOnTarget);
	AActor* GetManualLockOnTarget() const;

public:
	/**
	 * 새로운 후보들을 탐지할 필요가 있는 경우
	 * @param bWasLockOn 이전의 락온 여부
	 * @return 탐지할 필요가 있는지 여부
	 */
	bool ShouldUpdateTargetCandidates(bool bWasLockOn) const;

protected:
	// 예약된 락온 상태
	bool bIsLockOn;
	// 예약된 락온 대상 누적 오프셋 값
	int32 mTargetSwitchOffset;
	// 예약된 수동 락온 대상 지정 값
	TObjectPtr<AActor> mManualLockOnTarget;
};

/************************
  ULockOnSensingComponent
*************************/

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeLockOnState, bool, bIsLockOn);

/**
 * 락 온 대상을 감지하는 컴포넌트
 */
UCLASS()
class P_ASHEN_CROWN_API ULockOnSensingComponent : public USightTargetSensingComponent
{
	GENERATED_BODY()
	
public:
	ULockOnSensingComponent();

public:
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	/* 가상 함수 오버라이딩 */

	virtual bool IsActorValidCandidate(AActor* DetectedActor) const override;
	virtual void SensingCadidataes(TSet<TObjectPtr<AActor>>& OutCandidateActors) override;
	virtual AActor* SelectTargetFromCandidates(const TSet<TObjectPtr<AActor>>& CandidateActors) const override;

protected:
	/**
	 * 감지 결과를 받아 락온 타겟 컴포넌트를 동작시키는 함수
	 * @param PreTargetActor 이전 타겟 액터
	 * @param NextTargetActor 이후 타겟 액터
	 */
	UFUNCTION(BlueprintNativeEvent)
	void ChangeTargetComponent(AActor* PreTargetActor, AActor* NextTargetActor);
	virtual void ChangeTargetComponent_Implementation(AActor* PreTargetActor, AActor* NextTargetActor);

public:
	bool IsLockOn() const;
	ULockOnTargetComponent* GetTargetComponent() const;

public:
	/* 인풋에 의해 Tick에서 처리할 다음 동작을 저장하는 함수들 */

	/**
	 * 락온 대상 변경 함수
	 * @param TargetSwitchOffset 락온 대상 오프셋 값
	 */
	void SwitchLockOnTarget(int32 TargetSwitchOffset);
	/**
	 * 락온 기능 온오프 토글 함수
	 */
	void ToggleLockOn();
	/**
	 * 락온 대상 수동 지정 함수
	 * @param NextLockOnTarget 지정 대상 액터
	 */
	void SetManualLockOnTarget(TObjectPtr<AActor> NextLockOnTarget);

private:
	/**
	 * 예약된 누적 입력 값으로 틱 업데이트 여부를 결정하는 함수
	 */
	void EvaluateLockOnUpdate();
	/**
	 * 예약된 누적 입력 값을 다음 틱을 위해 초기화하는 함수
	 */
	void ResetPendingState();

public:
	// 락온 여부가 변경되었을 때 호출되는 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "LockOn")
	FOnChangeLockOnState OnToggleLockOn;

protected:
	// 락온이 따라가는 최대 거리
	// (감지 반경을 벗어나도 최대 거리까지 락온 적용)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LockOn")
	float mMaxLockOnDistance;

protected:
	// 현재 락온 상태 켜짐 여부
	bool bIsLockOn;

	// 예약된 락온 누적 인풋 데이터 값
	FPendingLockOnInput mPendingInput;

protected:
	TObjectPtr<ULockOnTargetComponent> mTargetComponent;
};
