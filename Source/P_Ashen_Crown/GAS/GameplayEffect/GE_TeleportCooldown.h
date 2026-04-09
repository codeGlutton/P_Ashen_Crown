// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_TeleportCooldown.generated.h"

/**
 * 보스의 텔레포트 스킬 쿨다운 이펙트
 */
UCLASS()
class P_ASHEN_CROWN_API UGE_TeleportCooldown : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_TeleportCooldown();
};
