// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BossMonster.h"
#include "BossLich.generated.h"

class ABossLichProjectile;
class ABossLichMagicFloor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDestroy, ABossLichMagicFloor*, MagicFloor);


/**
 * 보스 몬스터 Lich 클래스입니다
 * 발사체, 범위 공격, 순간이동 등 다양한 스킬을 사용합니다
 */
UCLASS()
class P_ASHEN_CROWN_API ABossLich : public ABossMonster
{
	GENERATED_BODY()

public:
	ABossLich();

protected:
	// 마법 발사체
	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<ABossLichProjectile> mProjectileClass;

	// 바닥 마법 액터 클래스
	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<ABossLichMagicFloor> mMagicFloorActorClass;

	// 이펙트
	/*UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TObjectPtr<UNiagaraSystem> mNiagara;*/

	// 실행 중인 바닥 마법 액터
	UPROPERTY()
	TObjectPtr<ABossLichMagicFloor> mMagicFloorActor;

	UPROPERTY(BlueprintReadWrite, Category = "SkillCooldown")
	float mProjectileCooldown = 3.f;

	UPROPERTY(BlueprintReadWrite, Category = "SkillCooldown")
	float mMagicFloorCooldown = 10.f;

	UPROPERTY(BlueprintReadWrite, Category = "SkillCooldown")
	float mTeleportCooldown = 15.f;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* mSkillFireSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* mFloorSkillSound;

public:
	UPROPERTY()
	FOnActorDestroy OnActorDestroy;

public:
	ABossLichMagicFloor* GetMagicFloor()
	{
		return mMagicFloorActor;
	}

	void SetMagicFloor(ABossLichMagicFloor* Floor)
	{
		mMagicFloorActor = Floor;
	}
public:
	// 일반 공격을 실행합니다.
	virtual void NormalAttack() override;

	// 기본 마법 발사 공격입니다.
	void UseProjectileSkill();

	// 몬스터 발 아래 범위 공격입니다. (데미지 실행)
	void UseMagicFloorSkill();

	// 범위 공격 액터 소환 (애니메이션 시작)
	void SpawnMagicFloorActor();

	// 순간이동 스킬입니다.
	void UseTeleportSkill();

	// 망 애니메이션 처리 함수입니다.
	virtual void HandleDeathAnimation() override;

	// Cooldown 상태를 블랙보드에 갱신합니다.
	void UpdateCooldownBlackboard();
};
