#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "UObject/NoExportTypes.h"

#include "UserSettings/EnhancedInputUserSettings.h"
#include "P_Ashen_Crown/SaveGame/SaveSound.h"

#include "UserGameSettingCommand.generated.h"

/************************
  FUserSettingCommandBase
*************************/

/**
 * 업캐스팅으로 관리를 위해 마련된 Command 베이스 객체
 */
struct FUserSettingCommandBase
{
public:
	virtual ~FUserSettingCommandBase();

	/**
	 * 현재 값으로 설정 커맨드 적용
	 */
	virtual void ApplyCommand() = 0;
	/**
	 * 기본 값으로 설정 커맨드 롤백
	 */
	virtual void RollbackCommand() = 0;
	/**
	 * 기본 값을 현재 값으로 치환
	 */
	virtual void CommitCommand() = 0;
};

/************************
	UUserSettingLinker
*************************/

/**
 * 유저 세팅 객체와 커맨드 사이에서 매개체로서의 역할을 하는 링크 객체 (일종의 어댑터 패턴)
 */
UCLASS(Abstract)
class P_ASHEN_CROWN_API UUserSettingLinker : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 세팅 값을 저장하는 함수
	 * @return 실질적 저장 여부
	 */
	virtual bool SaveSettings() PURE_VIRTUAL(, return false;);
	/**
	 * 세팅 값을 로드하여, 커스텀가능한 설정 커맨드를 추가해주는 함수
	 * @param Commands 설정 커맨드를 추가할 버퍼
	 */
	virtual void LoadSettings(TMap<FName, TUniquePtr<FUserSettingCommandBase>>& Commands) PURE_VIRTUAL(, );
};

/************************
   FUserSettingCommand
*************************/

/**
 * 롤백을 위해 설정 값을 저장해두는 객체
 * @tparam InValueType 설정 값 형식
 * @tparam InLinkType 링크 타입
 */
template<typename InValueType, typename InLinkType>
struct FUserSettingCommand : public FUserSettingCommandBase
{
public:
	using ValueType = InValueType;
	using LinkType = InLinkType;

public:
	FUserSettingCommand(ValueType DefaultValue, LinkType* Linker);

	const ValueType& GetDefaultValue() const;
	const ValueType& GetCurrenttValue() const;
	void SetCurrenttValue(const ValueType& NewValue);

	virtual void ApplyCommand() final;
	virtual void RollbackCommand() final;
	virtual void CommitCommand() final;

protected:
	/**
	 * 값을 게임 내 적용해주는 로직 함수
	 */
	virtual void ActivateCommand() = 0;

protected:
	ValueType mDefaultValue;
	ValueType mCurrentValue;

	TObjectPtr<LinkType> mLinker;
};

template<typename InValueType, typename InLinkType>
inline FUserSettingCommand<InValueType, InLinkType>::FUserSettingCommand(ValueType DefaultValue, LinkType* Linker) : mDefaultValue(DefaultValue), mCurrentValue(DefaultValue), mLinker(Linker)
{
}

template<typename InValueType, typename InLinkType>
inline const InValueType& FUserSettingCommand<InValueType, InLinkType>::GetDefaultValue() const
{
	return mDefaultValue;
}

template<typename InValueType, typename InLinkType>
inline const InValueType& FUserSettingCommand<InValueType, InLinkType>::GetCurrenttValue() const
{
	return mCurrentValue;
}

template<typename InValueType, typename InLinkType>
inline void FUserSettingCommand<InValueType, InLinkType>::SetCurrenttValue(const InValueType& NewValue)
{
	if (mCurrentValue == NewValue)
	{
		return;
	}
	mCurrentValue = NewValue;
	LinkType::mDirtyCount += mCurrentValue == mDefaultValue ? -1 : 1;
}

template<typename InValueType, typename InLinkType>
inline void FUserSettingCommand<InValueType, InLinkType>::ApplyCommand()
{
	ActivateCommand();
}

template<typename InValueType, typename InLinkType>
inline void FUserSettingCommand<InValueType, InLinkType>::RollbackCommand()
{
	if (mDefaultValue == mCurrentValue)
	{
		return;
	}
	mCurrentValue = mDefaultValue;
	ActivateCommand();
}

template<typename InValueType, typename InLinkType>
inline void FUserSettingCommand<InValueType, InLinkType>::CommitCommand()
{
	mDefaultValue = mCurrentValue;
}

/************************
UUserEnhancedInputSettingLinker
*************************/

struct FEnhancedInputKeyInfo
{
public:
	FName mMappingName;
	FName mDisplayName;
	FKey mNewKey;

	bool operator==(const FEnhancedInputKeyInfo& Other) const
	{
		return mMappingName == Other.mMappingName && mNewKey == Other.mNewKey;
	}
};

UCLASS()
class P_ASHEN_CROWN_API UUserEnhancedInputSettingLinker : public UUserSettingLinker
{
	GENERATED_BODY()

public:
	virtual bool SaveSettings() override;
	virtual void LoadSettings(TMap<FName, TUniquePtr<FUserSettingCommandBase>>& Commands) override;

	/**
	 * 키 입력값을 변경하는 함수
	 * @param KeySettingInfo 변경힐 키 정보
	 */
	void ChangeInput(const FEnhancedInputKeyInfo& KeySettingInfo);

public:
	static uint32 mDirtyCount;
};

/************************
FUserEnhancedInputSettingCommand
*************************/

struct FUserEnhancedInputSettingCommand : public FUserSettingCommand<FEnhancedInputKeyInfo, UUserEnhancedInputSettingLinker>
{
public:
	FUserEnhancedInputSettingCommand(ValueType DefaultValue, LinkType* Linker) : FUserSettingCommand(DefaultValue, Linker) {}

protected:
	virtual void ActivateCommand() override;
};

/************************
  UUserSoundSettingLinker
*************************/

USTRUCT(BlueprintType)
struct FSoundSettingInfo
{
	GENERATED_BODY()

public:
	ESoundSettingType SettingType;
	float Volume;

	bool operator==(const FSoundSettingInfo& Other) const
	{
		return SettingType == Other.SettingType && Volume == Other.Volume;
	}
};

UCLASS(Config = Game)
class P_ASHEN_CROWN_API UUserSoundSettingLinker : public UUserSettingLinker
{
	GENERATED_BODY()

public:
	UUserSoundSettingLinker();

public:
	virtual bool SaveSettings() override;
	virtual void LoadSettings(TMap<FName, TUniquePtr<FUserSettingCommandBase>>& Commands) override;

	/**
	 * 사운드 볼륨을 변경하는 함수
	 * @param SoundSettingInfo 변경힐 사운드 정보
	 */
	void ChangeSoundVolume(const FSoundSettingInfo& SoundSettingInfo);

public:
	static uint32 mDirtyCount;

protected:
	// 사용자 SaveSound 파일 이름. DefaultGame.ini에서 설정
	UPROPERTY(Config)
	FString mSaveSoundFileName;

	UPROPERTY()
	TObjectPtr<USaveSound> mCurSaveSound;
	UPROPERTY()
	TObjectPtr<USoundMix> mSoundMix;
	UPROPERTY()
	TArray<TObjectPtr<USoundClass>> mSoundClasses;
};

/************************
 FUserSoundSettingCommand
*************************/

struct FUserSoundVolumeSettingCommand : public FUserSettingCommand<FSoundSettingInfo, UUserSoundSettingLinker>
{
public:
	FUserSoundVolumeSettingCommand(ValueType DefaultValue, LinkType* Linker) : FUserSettingCommand(DefaultValue, Linker) {}

protected:
	virtual void ActivateCommand() override;
};

/************************
UUserGraphicsSettingLinker
*************************/

UCLASS()
class P_ASHEN_CROWN_API UUserGraphicsSettingLinker : public UUserSettingLinker
{
	GENERATED_BODY()

public:
	virtual bool SaveSettings() override;
	virtual void LoadSettings(TMap<FName, TUniquePtr<FUserSettingCommandBase>>& Commands) override;

	/**
	 * 해상도를 변경하는 함수
	 * @param Resolution 변경할 해상도
	 */
	void ChangeScreenResolution(FIntPoint Resolution);

public:
	static uint32 mDirtyCount;
};

/************************
FUserScreenResolutionSettingCommand
*************************/

struct FUserScreenResolutionSettingCommand : public FUserSettingCommand<FIntPoint, UUserGraphicsSettingLinker>
{
public:
	FUserScreenResolutionSettingCommand(ValueType DefaultValue, LinkType* Linker) : FUserSettingCommand(DefaultValue, Linker) {}

protected:
	virtual void ActivateCommand() override;
};
