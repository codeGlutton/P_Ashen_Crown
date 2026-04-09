// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Monster/MonsterInfo.h"
#include "UObject/NoExportTypes.h"
#include "MonsterData.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UMonsterData : public UObject
{
	GENERATED_BODY()
	
public:
	UMonsterData();

private:
	// 몬스터 능력치 테이블
	TObjectPtr<UDataTable>	mInfoTable;
	// 순간이동 위치 테이블
	TObjectPtr<UDataTable> mTeleportTable;

public:
	// 몬스터 이름에 해당하는 정보를 찾아 반환해준다.
	// @param Name 몬스터 이름
	FMonsterTableInfo* FindMonsterInfo(const FName& Name)	const;

	// TeleportTable에 등록된 행 중에 하나를 랜덤으로 얻어온다.
	// @param CurrentLocation 현재 몬스터의 위치
	// @return 유효한 위치 정보 포인터, 실패 시 nullptr
	FBossTeleportLocation* GetRandomTeleportLocation(const FVector& CurrentLocation) const;
};
