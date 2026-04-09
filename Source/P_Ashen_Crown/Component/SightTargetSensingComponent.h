#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "SightTargetSensingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeTargetActor, AActor*, PreTargetActor, AActor*, NextTargetActor);

/**
 * 시야에 보이는 타겟을 감지하는 컴포넌트
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Abstract)
class P_ASHEN_CROWN_API USightTargetSensingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USightTargetSensingComponent();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetTraceChannel(ECollisionChannel TraceChannel);
	ECollisionChannel GetTraceChannel() const;

	void SetBlockTraceChannel(ECollisionChannel BlockTraceChannel);
	ECollisionChannel SetBlockTraceChannel() const;

	void SetTraceTag(const FName& TraceTag);
	const FName& GetTraceTag() const;

	void SetTeamFilter(FAISenseAffiliationFilter TeamFilter);
	FAISenseAffiliationFilter GetTeamFilter() const;

	void SetQuerySphereRadius(float QuerySphereRadius);
	float GetQuerySphereRadius() const;

	void SetQueryHalfDegree(float QueryHalfDegree);
	float GetQueryHalfDegree() const;

	void SetQueryForwardOffset(float QueryForwardOffset);
	float GetQueryForwardOffset() const;

	AActor* GetTargetActor() const;

protected:
	/**
	 * 후보들 중 유효한 대상 필터링
	 * @param DetectedActor 탐지된 액터
	 * @return 후보로 포함할지 여부 
	 */
	virtual bool IsActorValidCandidate(AActor* DetectedActor) const PURE_VIRTUAL(, return true; );
	/**
	 * 시야에 따라 타겟이 될 수 있는 후보들을 탐지하는 함수
	 * @param OutCandidateActors 새로운 후보들 결과
	 */
	virtual void SensingCadidataes(TSet<TObjectPtr<AActor>>& OutCandidateActors);
	/**
	 * 최종 타겟 선택
	 * @param CandidateActors 후보 액터들
	 * @return 선택된 액터
	 */
	virtual AActor* SelectTargetFromCandidates(const TSet<TObjectPtr<AActor>>& CandidateActors) const PURE_VIRTUAL(, return nullptr; );
	/**
	 * 새로운 액터 탐지시 호출되는 함수
	 * @param CandidateActor 진입한 액터
	 */
	virtual void EnterCandidateActor(AActor* CandidateActor) {};
	/**
	 * 사라진 액터 탐지시 호출되는 함수
	 * @param CandidateActor 나간 액터
	 */
	virtual void LeaveCandidateActor(AActor* CandidateActor) {};

public:
	// 타겟 액터가 변경될 때 호출되는 대리자
	UPROPERTY(BlueprintAssignable, Category = "SightTargetSensingComponent")
	FOnChangeTargetActor OnChangeTargetActor;

protected:
	// 감지할 채널 타입 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SightTargetSensingComponent", meta = (DisplayName = "대상 탐지 트레이스 채널"))
	TEnumAsByte<ECollisionChannel> mTraceChannel;

	// 블록을 감지할 채널 타입 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SightTargetSensingComponent", meta = (DisplayName = "블록킹 탐지 트레이스 채널"))
	TEnumAsByte<ECollisionChannel> mBlockTraceChannel;

	// 감지할 액터 태그 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SightTargetSensingComponent", meta = (DisplayName = "탐지할 액터 태그"))
	FName mTraceTag;

	// 감지할 팀 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SightTargetSensingComponent", meta = (DisplayName = "탐지할 팀 관계"))
	FAISenseAffiliationFilter mTeamFilter;

	// 감지 범위 반지름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SightTargetSensingComponent", meta = (DisplayName = "탐지 반지름 길이"))
	float mQuerySphereRadius;

	// 감지 범위 탐지 각도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SightTargetSensingComponent", meta = (DisplayName = "탐지 절반 각도"))
	float mQueryHalfDegree;

	// 눈 위치의 앞 뒤 방향 Offset
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SightTargetSensingComponent", meta = (DisplayName = "탐지 시작 위치 오프셋", TooolTip = "기본값은 눈 위치"))
	float mQueryForwardOffset;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SightTargetSensingComponent")
	TObjectPtr<AActor> mTargetActor;
	TSet<TObjectPtr<AActor>> mCandidateActors;

	// 후보자 액터 업데이트
	bool bUpdateCandidateActors;
	// 타겟 액터 업데이트
	bool bUpdateTargetActor;

private:
	TSet<TObjectPtr<AActor>> mNextCandidateActors;
};
