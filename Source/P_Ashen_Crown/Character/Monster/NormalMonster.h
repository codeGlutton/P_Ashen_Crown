// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MonsterBase.h"
#include "NormalMonster.generated.h"

/**
 * 일반 몬스터 클래스
 * AMonsterBase를 상속받으며, 기본 공격 및 피격 반응, 복귀 처리 기능을 한다.
 */
UCLASS()
class P_ASHEN_CROWN_API ANormalMonster : public AMonsterBase
{
	GENERATED_BODY()
	
public:
	ANormalMonster();

protected:
	UPROPERTY(EditAnywhere)
	float mHitMoveDistance = 300.f;

	bool bHasDetectedTarget = false;

	// 처음 위치
	FVector mOriginalLocation;

	FTimerHandle mReturnCheckHandle;

private:
	// 일반 공격 실제 데미지 처리합니다.
	virtual void NormalAttack() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	// 피격 반응 종료 시 호출합니다.
	// @param Attacker 공격한 액터 (방향 계산 등에 사용)
	virtual void OnHitReactionEnd(AActor* Attacker) override;

	// 일정 시간 후 원래 위치로 되돌아가는지 체크하는 함수
	// 몬스터가 타겟을 잃었거나 일정 거리 이상 벗어난 경우 호출합니다.
	void CheckReturnToOriginalLocation();

public:
	// 복귀 타이머를 해제하는 함수
	void ClearReturnTimer();
};
