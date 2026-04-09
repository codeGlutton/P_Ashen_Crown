// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MonsterInfo.h"
#include "../MonsterAnimInstance.h"
#include "BossMonsterAnimInstance.generated.h"

/**
 * BossMonster 전용 Animation처리 클래스
 */
UCLASS()
class P_ASHEN_CROWN_API UBossMonsterAnimInstance : public UMonsterAnimInstance
{
	GENERATED_BODY()
	
public:
	UBossMonsterAnimInstance();
	
public:
	virtual void AnimNotify_AttackHit() override;

	virtual void AnimNotify_AttackEnd() override;
	// 애니메이션 노티파이에서 호출되어 보스의 스킬1 발사 타이밍을 전달합니다.
	UFUNCTION()
	virtual void AnimNotify_Skill1Fire();

	UFUNCTION()
	virtual void AnimNotify_SkillStart();

	UFUNCTION()
	void AnimNotify_MagicFloor();

	UFUNCTION()
	void AnimNotify_SkillEnd();

	UFUNCTION()
	void AnimNotify_Teleport();

	UFUNCTION()
	void AnimNotify_TeleportEnd();

	virtual void PlayHit(AActor* Attacker) override;

};
