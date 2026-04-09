// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Engine/OverlapResult.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"

#include "Delegates/Delegate.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTTaskNode.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"

#include "Kismet/KismetMathLibrary.h"

#include "Navigation/PathFollowingComponent.h"

#include "MonsterInfo.generated.h"

/**
 * 
 */
FRotator GetTargetRotator(const FVector& Target, const FVector& Current);
FRotator GetTargetRotatorYaw(FVector Target, FVector Current);


// 피격 방향
UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	None,
	Front,
	Back,
	Left,
	Right
};

// 피격 방향 계산
// @param Victim 피격 받은 액터(몬스터)
// @param Attacker 공격한 액터(플레이어)
EHitDirection GetHitDirection(AActor* Victim, AActor* Attacker);

// 몽타주용 enum
UENUM(BlueprintType)
enum class EMonsterAnim : uint8
{
	Idle,
	Walk,
	Run,
	Attack,
	Attack2,
	Signal1,
	Skill1,
	Skill2,
	Skill3,
	Death
};

// 몬스터 종류
UENUM(BlueprintType)
enum class MonsterType : uint8
{
	None,
	Griffon,
	Lich,
	Golem
};

// 몬스터 테이블 정보 구조체
// UMonsterData에서 참조
USTRUCT(BlueprintType)
struct FMonsterTableInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString			Name;

	UPROPERTY(EditAnywhere)
	float			Attack;

	UPROPERTY(EditAnywhere)
	float			Defense;

	UPROPERTY(EditAnywhere)
	float			HP;

	UPROPERTY(EditAnywhere)
	float			MaxHP;

	UPROPERTY(EditAnywhere)
	float			AttackDistance;

	UPROPERTY(EditAnywhere)
	float			DetectDistance;

	UPROPERTY(EditAnywhere)
	float			MoveSpeed;

	UPROPERTY(EditAnywhere)
	float			SpellDamage;
};

// 순간이동 테이블 정보 구조체
// UMonsterData에서 참조
USTRUCT(BlueprintType)
struct FBossTeleportLocation : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TeleportLocation;
};

// 몬스터 블랙보드 키
namespace MonsterAIKey
{
	const FName TargetActor = TEXT("TargetActor");
	const FName IsInAttackRange = TEXT("IsInAttackRange");
	const FName AttackEnd = TEXT("AttackEnd");
	const FName AttackTarget = TEXT("AttackTarget");
	const FName IsDead = TEXT("IsDead");
	const FName SkillEnd = TEXT("SkillEnd");
	const FName TeleportEnd = TEXT("TeleportEnd");
}