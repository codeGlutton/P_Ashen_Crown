// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterData.h"

UMonsterData::UMonsterData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>
		DataAsset(TEXT("/Script/Engine.DataTable'/Game/AC/Data/Monster/DT_MonsterInfo.DT_MonsterInfo'"));
	if (DataAsset.Succeeded())
		mInfoTable = DataAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>
		TeleportAsset(TEXT("/Script/Engine.DataTable'/Game/AC/Data/Monster/DT_TeleportLocation.DT_TeleportLocation'"));
	if (TeleportAsset.Succeeded())
		mTeleportTable = TeleportAsset.Object;
}

FMonsterTableInfo* UMonsterData::FindMonsterInfo(const FName& Name)	const
{
	if (!mInfoTable)
		return nullptr;

	return mInfoTable->FindRow<FMonsterTableInfo>(Name, TEXT(""));
}

FBossTeleportLocation* UMonsterData::GetRandomTeleportLocation(const FVector& CurrentLocation) const
{
	if (!mTeleportTable) 
		return nullptr;

	// 
	const float MinDistSq = FMath::Square(100.f);

	// 테이블의 RowName 리스트를 얻어언다.
	TArray<FName> RowNames = mTeleportTable->GetRowNames();
	if (RowNames.Num() == 0) 
		return nullptr;

	for (int32 i = 0; i < 10; ++i)
	{
		FName SelectedRow = RowNames[FMath::RandRange(0, RowNames.Num() - 1)];
		FBossTeleportLocation* Row = mTeleportTable->FindRow<FBossTeleportLocation>(SelectedRow, TEXT(""));

		if (Row && FVector::DistSquared2D(CurrentLocation, Row->TeleportLocation) > MinDistSq)
			return Row;
	}

	return nullptr;

	// 랜덤 RowName 선택
	/*int32 Index = FMath::RandRange(0, RowNames.Num() - 1);
	FName SelectedRow = RowNames[Index];

	return mTeleportTable->FindRow<FBossTeleportLocation>(SelectedRow, TEXT(""));*/
}
