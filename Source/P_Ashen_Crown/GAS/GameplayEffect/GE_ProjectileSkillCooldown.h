// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_ProjectileSkillCooldown.generated.h"

/**
 * 보스의 발사체 스킬 쿨다운 이펙트
 */
UCLASS()
class P_ASHEN_CROWN_API UGE_ProjectileSkillCooldown : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_ProjectileSkillCooldown();
	
};
