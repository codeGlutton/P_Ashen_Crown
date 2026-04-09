#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "GameFramework/WorldSettings.h"
#include "P_Ashen_Crown/Setting/SharedViewSettings.h"
#include "P_Ashen_Crown/Singleton/Subsystem/GameEventSubsystem.h"
#include "ACWorldSettings.generated.h"

/**
 * DataTable 타입 종류
 */
UENUM(BlueprintType)
enum class EDataTableType : uint8
{
	Map UMETA(ToolTip = "맵에 대한 정보 for UI"),	
	CharacterStat,
	MonsterStat,
	CharacterSkill,
	MonsterSkill,
	Equip,
	Potion,
	Rune,
	EnhanceObj UMETA(ToolTip = "강화 재료에 따른 강화율표")
};

/**
 * Ashen Crown에서 사용할 WorldSettigs
 */
UCLASS()
class P_ASHEN_CROWN_API AACWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
public:
	/**
	 * WorldSetting에서 원하는 Data Table Row를 찾기 위한 함수
	 * 
	 * @tparam 반환 TableRow타입
	 * @param TableType 데이터 테이블 종류
	 * @param Key 테이블 내 Row를 찾기 위한 Key 값
	 * @return T*로 캐스팅한 FTableRowBase* 값
	 */
	template<typename T>
	T* GetDataTableRow(EDataTableType TableType, const FName& Key);
	const TMap<FName, FMVVMViewInfo>& GetWorldSharedViews() const;
	const TMap<FName, FGameEventContext>& GetWorldGameEvents() const;

	const FString& GetStageName() const;

protected:
	/**
	 * UI에 표기할 스테이지 이름
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WorldSettings|Data")
	FString mStageName;

	/**
	 * 해당 월드에서 사용할 DataTable들
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WorldSettings|Data")
	TMap<EDataTableType, TObjectPtr<UDataTable>> mDataTables;

	/**
	 * 해당 월드에서 사용할 SharedWidget들
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WorldSettings|View")
	TMap<FName, FMVVMViewInfo> mWorldSharedViews;

	/**
	 * 해당 월드에서 사용할 게임 이벤트들
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WorldSettings|GameEvent")
	TMap<FName, FGameEventContext> mWorldGameEvents;
};

template<typename T>
inline T* AACWorldSettings::GetDataTableRow(EDataTableType TableType, const FName& Key)
{
	if (mDataTables.Find(TableType) != nullptr)
	{
		return mDataTables[TableType]->FindRow<T>(Key, TEXT(""));
	}
	return nullptr;
}
