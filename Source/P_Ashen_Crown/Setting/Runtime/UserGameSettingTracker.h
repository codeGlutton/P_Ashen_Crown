#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "UObject/NoExportTypes.h"
#include "P_Ashen_Crown/Setting/Runtime/UserGameSettingCommand.h"
#include "UserGameSettingTracker.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCancelSetting);

class UUserSettingLinker;

/**
 * 유저 세팅 객체와 연결하는 링크 객체에 연관된 커맨드 집합 구조체
 */
struct FLinkCommandRow
{
public:
	TMap<FName, TUniquePtr<FUserSettingCommandBase>> mCommands;
};

/**
 * 유저 세팅 변경을 적용 및 감시하고, 롤백하거나 저장하는 Tracker 클래스
 */
UCLASS()
class P_ASHEN_CROWN_API UUserGameSettingTracker : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 설정을 담당하는 객체와 연결해주는 Linker를 등록하고, 자동으로 관련 Command를 추가해주는 함수
	 * @tparam LinkType 특정 링크 타입 (ex. EnhancedInput 설정 연결 객체, GameUserSetting 설정 연결 객체)
	 */
	template<typename LinkType>
	void RegisterLinker();

	/**
	 * 특정 커맨드를 탐색해서 현재의 값을 가져오는 함수
	 * @tparam CommandType 확인할 커맨드 타입
	 * @return CommandType::ValueType 현재 값
	 */
	template<typename CommandType>
	CommandType::ValueType GetCurrentValue(const FName& Key);

	/**
	 * 링크에 연관된 모든 커맨드 소유 객체를 반환하는 함수
	 * @tparam LinkType 특정 링크 타입 (ex. EnhancedInput 설정 연결 객체, GameUserSetting 설정 연결 객체)
	 * @return 연관 커맨드 소유 객체
	 */
	template<typename LinkType>
	const FLinkCommandRow* GetLinkCommands();

	/**
	 * 특정 커맨드를 탐색해서 새로운 값을 적용하는 함수 (설정은 적용되지만, 파일로 저장되지는 않음)
	 * @tparam CommandType 적용할 커맨드 타입
	 * @param Key 특정 커맨드를 파악하기 위한 FName 키 값
	 * @param NewValue 커맨드에 적용할 새로운 값
	 */
	template<typename CommandType>
	void ApplyCommand(const FName& Key, typename const CommandType::ValueType& NewValue);

public:
	/**
	 * 모든 세팅 값을 저장하는 함수
	 */
	void SaveSettings();
	/**
	 * 모두 기존 값으로 되돌리는 함수
	 */
	void CancelSettings();

public:
	FOnCancelSetting OnCancelSetting;

protected:
	TMap<TSubclassOf<UUserSettingLinker>, FLinkCommandRow> mCommandRows;
	UPROPERTY()
	TMap<TSubclassOf<UUserSettingLinker>, TObjectPtr<UUserSettingLinker>> mLinkers;
};

template<typename LinkType>
inline void UUserGameSettingTracker::RegisterLinker()
{
	auto* LinkerClassType = LinkType::StaticClass();
	if (mCommandRows.Contains(LinkerClassType) == true)
	{
		return;
	}
	mCommandRows.Add(LinkerClassType, FLinkCommandRow());

	auto* LinkerObj = NewObject<LinkType>(this);
	mLinkers.Emplace(LinkerClassType, LinkerObj);
	LinkerObj->LoadSettings(mCommandRows[LinkerClassType].mCommands);
}

template<typename CommandType>
inline CommandType::ValueType UUserGameSettingTracker::GetCurrentValue(const FName& Key)
{
	const FLinkCommandRow* CommandRow = mCommandRows.Find(typename CommandType::LinkType::StaticClass());
	if (CommandRow == nullptr)
	{
		return CommandType::ValueType();
	}
	const TUniquePtr<FUserSettingCommandBase>* Command = CommandRow->mCommands.Find(Key);
	if (Command == nullptr)
	{
		return CommandType::ValueType();
	}
	auto* TargetCommand = StaticCast<FUserSettingCommand<typename CommandType::ValueType, typename CommandType::LinkType>*>(Command->Get());
	if (TargetCommand == nullptr)
	{
		return CommandType::ValueType();
	}
	return TargetCommand->GetCurrenttValue();
}

template<typename LinkType>
inline const FLinkCommandRow* UUserGameSettingTracker::GetLinkCommands()
{
	return mCommandRows.Find(LinkType::StaticClass());
}

template<typename CommandType>
inline void UUserGameSettingTracker::ApplyCommand(const FName& Key, typename const CommandType::ValueType& NewValue)
{
	const FLinkCommandRow* CommandRow = mCommandRows.Find(typename CommandType::LinkType::StaticClass());
	if (CommandRow == nullptr)
	{
		return;
	}
	const TUniquePtr<FUserSettingCommandBase>* Command = CommandRow->mCommands.Find(Key);
	if (Command == nullptr)
	{
		return;
	}
	auto* TargetCommand = StaticCast<FUserSettingCommand<typename CommandType::ValueType, typename CommandType::LinkType>*>(Command->Get());
	if (TargetCommand == nullptr)
	{
		return;
	}
	TargetCommand->SetCurrenttValue(NewValue);
	TargetCommand->ApplyCommand();
}
