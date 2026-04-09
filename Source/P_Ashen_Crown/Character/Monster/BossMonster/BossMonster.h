// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MonsterBase.h"
#include "BossMonster.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API ABossMonster : public AMonsterBase
{
	GENERATED_BODY()

public:
	ABossMonster();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Boss")
	int32 mCurrentPhase = 1;

	// 체력 30% 이하로 내려가면 2페이즈
	UPROPERTY(EditAnywhere, Category = "Boss")
	float mPhase2Threshold = 0.3f;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
