// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MonsterInfo.h"
#include "../../Character/BaseCharacter.h"
#include "../../GAS/AttributeSet/MonsterAttributeSet.h"
#include "GameFramework/Character.h"
#include "MonsterBase.generated.h"

UCLASS(Abstract)
class P_ASHEN_CROWN_API AMonsterBase : public ABaseCharacter
{
	GENERATED_BODY()

public:

	AMonsterBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> mBehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBlackboardData>	mBlackboardData;

	UPROPERTY()
	TObjectPtr<class UMonsterAnimInstance> mMonsterAnim;

	UPROPERTY(EditAnywhere)
	FName			mDataKey;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UMonsterAttributeSet> mMonsterAttribute;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UBaseAttributeSet> mBaseAttribute;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDead = false;

	UPROPERTY()
	bool bIsHitReacting = false;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector>		mPatrolPoints;

	int32				mPatrolIndex = 1;

protected:
	// 락온 대상 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LockOn")
	TObjectPtr<class ULockOnTargetActorComponent> mLockOnTargetActorComponent;

	// 락온 UI를 띄워줄 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LockOn")
	TObjectPtr<class UWidgetComponent> mLockOnWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LockOn")
	FName mLockOnViewModelName;

protected:
	// 체력바 UI를 띄워줄 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
	TObjectPtr<class UWidgetComponent> mHPWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	FName mHPViewModelName;

public:
	// 각 몬스터 마다 개별 시야 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight")
	float mSightRadius = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight")
	float mLoseSightRadius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight")
	float mPeripheralVisionAngle = 90.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool bLockRotation = false;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform);

public:
	UBehaviorTree* GetBehaviorTree() const
	{
		return mBehaviorTree;
	}

	UBlackboardData* GetBlackboardData() const
	{
		return mBlackboardData;
	}

	UFUNCTION(BlueprintCallable)
	UMonsterAnimInstance* GetMonsterAnim() const
	{ 
		return mMonsterAnim; 
	}

	UFUNCTION()
	UMonsterAttributeSet* GetMonsterAttribute() const
	{
		return mMonsterAttribute;
	}

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor() const;

	const FVector& GetPatrolPoint()	const
	{
		return mPatrolPoints[mPatrolIndex];
	}

	void NextPatrolPoint()
	{
		mPatrolIndex = (mPatrolIndex + 1) % mPatrolPoints.Num();
	}

public:
	// 애니메이션 전환
	void ChangeAnim(EMonsterAnim Anim);

public:	
	// 실제로 일반 공격을 실행하는 함수입니다.
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void NormalAttack();

	// 다음 섹션의 공격 애니메이션 몽타주를 재생합니다.
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void PlayAttackMontageSection();

	// 공격 받았을 때 애니메이션을 처리합니다.
	// @param Attacker 공격한 액터
	// @param HitResult 충돌 정보
	virtual void OnDamageTaken(AActor* Attacker, const FHitResult* HitResult = nullptr);

	// 몬스터 사망 시 호출합니다.
	// @param Tag 적용된 GameplayTag
	// @param TagCount 현재 태그의 개수 (0이면 제거된 상태)
	UFUNCTION()
	void OnDeath(const FGameplayTag Tag, int32 TagCount);

	// 피격 방향에 반응하여 회전하거나 이동합니다.
	// @param Attacker 공격한 액터
	// @param HitResult 충돌 정보
	virtual void ApplyHitReaction(AActor* Attacker, const FHitResult* HitResult);

	// 사망 애니메이션 재생합니다.
	virtual void HandleDeathAnimation();

	// 피격 애니메이션이 끝난 후 회전, 이동 등을 실행합니다.
	// @param Attacker 마지막으로 공격한 액터
	virtual void OnHitReactionEnd(AActor* Attacker);

	// Actor 삭제
	void DestroyMonster();

	// 상태 전환 및 사망 처리
	void RagDoll();
};
