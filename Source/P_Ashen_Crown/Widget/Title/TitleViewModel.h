#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/Widget/ComposableViewModelBase.h"
#include "Framework/Commands/InputChord.h"
#include "TitleViewModel.generated.h"

/************************
  ULoadGameSlotViewModel
*************************/

class USavePlayer;

/**
 * 게임 로드창 슬롯에 대한 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API ULoadGameSlotViewModel : public UComposableViewModelBase
{
	GENERATED_BODY()

public:
	ULoadGameSlotViewModel();

public:
	/* FieldNotify Getter/Setter */

	void SetSavePlayer(USavePlayer* SavePlayer);
	USavePlayer* GetSavePlayer() const;

	void SetStageName(const FText& StageName);
	const FText& GetStageName() const;

	void SetStageImage(UTexture2D* StageImage);
	UTexture2D* GetStageImage() const;

	void Select(bool bIsWillSelected);
	bool IsSelected() const;

public:
	/* FieldNotify 함수 */

	UFUNCTION(BlueprintPure, FieldNotify)
	int32 GetLevel() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	FDateTime GetDate() const;

public:
	/* 수동 실행 함수 */

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetSavePlayer", Getter = "GetSavePlayer", meta = (AllowPrivateAccess))
	TObjectPtr<USavePlayer> mSavePlayer;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetStageName", Getter = "GetStageName", meta = (AllowPrivateAccess))
	FText mStageName;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetStageImage", Getter = "GetStageImage", meta = (AllowPrivateAccess))
	TObjectPtr<UTexture2D> mStageImage;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "Select", Getter = "IsSelected", meta = (AllowPrivateAccess))
	bool bIsSelected;
};

/************************
	ULoadGameViewModel
*************************/

/**
 * 게임 로드창에 대한 ViewModel
 */

UCLASS()
class P_ASHEN_CROWN_API ULoadGameViewModel : public UComposableViewModelBase
{
	GENERATED_BODY()

public:
	ULoadGameViewModel();

public:
	/* FieldNotify Getter/Setter */

	void SetSelectedSlotVM(ULoadGameSlotViewModel* SelectedSlotVM);
	ULoadGameSlotViewModel* GetSelectedSlotVM() const;

protected:
	/* Sub ViewModel에 대한 FieldNotify */

	/**
	 * Slot ViewModel이 bSelected 변수가 변경되었을 때 알림 함수
	 * @param Obj SlotViewModel 객체
	 * @param FieIdId 변경된 필드 리플렉션 id
	 */
	void NotifyFromSelectedSlot(UObject* Obj, UE::FieldNotification::FFieldId FieIdId);
	TArray<FDelegateHandle> mSelectedSlotNotifyHandles;

public:
	/* 수동 실행 함수 */

	virtual void Initialize_Implementation() override;
	virtual void Deinitialize_Implementation() override;

	/**
	 * 선택된 (USaveGame이 담긴) Slot을 제거
	 */
	UFUNCTION(BlueprintCallable, Category = "LoadGameViewModel")
	void DeleteSelectedSlot();

	/**
	 * 선택된 (USaveGame이 담긴) Slot을 통해 게임 시작
	 */
	UFUNCTION(BlueprintCallable, Category = "LoadGameViewModel")
	void PlayGameUsingSelectedSlot();

public:
	/* 바인딩 함수 */

	/**
	 * SaveGameSubsystem에서 데이터 변동 시, 처리할 함수
	 * @param PlayerSaveList 변동된 SavePlayer 데이터들
	 */
	void UpdateLoadGameSlotVM(const TDoubleLinkedList<TObjectPtr<USavePlayer>>& PlayerSaveList);

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, meta = (AllowPrivateAccess))
	TArray<TObjectPtr<ULoadGameSlotViewModel>> mLoadGameSlotVMs;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetSelectedSlotVM", Getter = "GetSelectedSlotVM", meta = (AllowPrivateAccess))
	TObjectPtr<ULoadGameSlotViewModel> mSelectedSlotVM;
};

/************************
   UOptionSlotViewModel
*************************/

/**
 * 게임 옵션 슬롯에 대한 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UOptionSlotViewModel : public UComposableViewModelBase
{
	GENERATED_BODY()

public:
	/* FieldNotify Getter/Setter */

	void Select(bool bIsWillSelected);
	bool IsSelected() const;

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "Select", Getter = "IsSelected", meta = (AllowPrivateAccess))
	bool bIsSelected;
};

/************************
   UOptionKeyViewModel
*************************/

/**
 * 게임 옵션 키 설정 슬롯에 대한 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UOptionKeyViewModel : public UComposableViewModelBase
{
	GENERATED_BODY()

public:
	/* FieldNotify Getter/Setter */

	void SetDisplayName(const FText& DisplayName);
	const FText& GetDisplayName() const;

	void SetMappingName(const FText& MappingName);
	const FText& GetMappingName() const;

	void SetKey(const FKey& Key);
	const FKey& GetKey() const;

public:
	/* FieldNotify 함수 */

	/**
	 * 선택된 키 정보 반환 함수
	 * @return 선택된 키
	 */
	UFUNCTION(BlueprintPure, FieldNotify)
	FInputChord GetSelectedKey() const;

public:
	/* 수동 실행 함수 */

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetDisplayName", Getter = "GetDisplayName", meta = (AllowPrivateAccess))
	FText mDisplayName;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetMappingName", Getter = "GetMappingName", meta = (AllowPrivateAccess))
	FText mMappingName;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetKey", Getter = "GetKey", meta = (AllowPrivateAccess))
	FKey mKey;
};

/************************
	UOptionViewModel
*************************/

UENUM(BlueprintType)
enum class EOptionState : uint8
{
	DisplayScreen = 0,
	CameraScreen,
	InputScreen,
	SoundScreen,
};

/**
 * 게임 옵션창에 대한 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UOptionViewModel : public UComposableViewModelBase
{
	GENERATED_BODY()

public:
	/* FieldNotify Getter/Setter */

	void SetSelectedSlotVM(UOptionSlotViewModel* SelectedSlotVM);
	UOptionSlotViewModel* GetSelectedSlotVM() const;

	void SetState(EOptionState State);
	EOptionState GetState() const;

	void SetScreenResolution(const FString& ScreenResolution);
	const FString& GetScreenResolution() const;

	void SetMasterVolume(float MasterVolume);
	float GetMasterVolume() const;

	void SetSFXVolume(float SFXVolume);
	float GetSFXVolume() const;

	void SetVoiceVolume(float VoiceVolume);
	float GetVoiceVolume() const;

	void SetBGMVolume(float BGMVolume);
	float GetBGMVolume() const;

public:
	/* FieldNotify 함수 */

	/**
	 * 옵션창에 띄워주어야할 설정 페이지 인덱스를 출력하는 함수
	 * @return 설정 페이지 인덱스
	 */
	UFUNCTION(BlueprintPure, FieldNotify)
	int32 GetOptionSwitchIndex() const;

protected:
	/* Sub ViewModel에 대한 FieldNotify */

	/**
	 * Slot ViewModel이 bSelected 변수가 변경되었을 때 알림 함수
	 * @param Obj SlotViewModel 객체
	 * @param FieIdId 변경된 필드 리플렉션 id
	 */
	void NotifyFromSelectedSlot(UObject* Obj, UE::FieldNotification::FFieldId FieIdId);
	TArray<FDelegateHandle> mSelectedSlotNotifyHandles;

public:
	/* 수동 실행 함수 */

	/**
	 * 현 위젯의 ViewModel에서 참조하기 위해 SubViewModel을 내부에 초기화하는 함수
	 * @param SubViewModels 내부의 모든 ViewModel들
	 */
	UFUNCTION(BlueprintCallable, Category = "OptionViewModel")
	void ConnectSubViewModel(const TArray<UOptionSlotViewModel*>& SubViewModels);
	virtual void Initialize_Implementation() override;
	virtual void Deinitialize_Implementation() override;

	/**
	 * 설정 저장 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "OptionViewModel")
	void SaveSetting();

	/**
	 * 설정 취소 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "OptionViewModel")
	void CancelSetting();

public:
	/**
	 * 게임 세팅에 맞추어 다시 ViewModel 값들을 초기화해주는 함수
	 */
	void RefreshGameSettingOutfit();

private:
	TArray<TObjectPtr<UOptionSlotViewModel>> mOptionSlotVMs;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetSelectedSlotVM", Getter = "GetSelectedSlotVM", meta = (AllowPrivateAccess))
	TObjectPtr<UOptionSlotViewModel> mSelectedSlotVM;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetState", Getter = "GetState", meta = (AllowPrivateAccess))
	EOptionState mState;

	UPROPERTY(BlueprintReadWrite, FieldNotify, meta = (AllowPrivateAccess))
	TArray<TObjectPtr<UOptionKeyViewModel>> mKeyVMs;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetScreenResolution", Getter = "GetScreenResolution", meta = (AllowPrivateAccess))
	FString mScreenResolution;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetMasterVolume", Getter = "GetMasterVolume", meta = (AllowPrivateAccess))
	float mMasterVolume;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetSFXVolume", Getter = "GetSFXVolume", meta = (AllowPrivateAccess))
	float mSFXVolume;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetVoiceVolume", Getter = "GetVoiceVolume", meta = (AllowPrivateAccess))
	float mVoiceVolume;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetBGMVolume", Getter = "GetBGMVolume", meta = (AllowPrivateAccess))
	float mBGMVolume;
};

/************************
	 UTitleViewModel
*************************/

UENUM(BlueprintType)
enum class ETitleState : uint8
{
	FirstScreen = 0,
	SelectMenu,
	NewGame,
	LoadGame,
	OptionGame,
};

/**
 * 타이틀 메뉴에 대한 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UTitleViewModel : public UComposableViewModelBase
{
	GENERATED_BODY()

public:
	UTitleViewModel();
	
public:
	/* FieldNotify Getter/Setter */

	void SetState(ETitleState State);
	ETitleState GetState() const;

	void SetHasPressedKey(bool bIsPressedKey);
	bool HasPressedKey() const;

public:
	/* FieldNotify 함수 */

	/**
	 * 하나의 SavePlayer라도 존재하는지를 파악하는 함수
	 * @return SavePlayer 여부
	 */
	UFUNCTION(BlueprintPure, FieldNotify)
	bool HasAnySavePlayer() const;

public:
	/* 수동 실행 함수 */

	virtual void Initialize_Implementation() override;
	virtual void Deinitialize_Implementation() override;

	/**
	 * (USaveGame이 담긴) Slot을 생성해 게임 시작
	 */
	UFUNCTION(BlueprintCallable, Category = "TitleViewModel")
	void StartNewGame();

	/**
	 * 가장 최신 (USaveGame이 담긴) Slot을 통해 게임 시작
	 */
	UFUNCTION(BlueprintCallable, Category = "TitleViewModel")
	void ContinueGame();

	/**
	 * 저장된 (USaveGame이 담긴) Slot을 선택해 게임 시작
	 */
	UFUNCTION(BlueprintCallable, Category = "TitleViewModel")
	void LoadGame();

	/**
	 * 게임에 대한 값을 설정
	 */
	UFUNCTION(BlueprintCallable, Category = "TitleViewModel")
	void ConfigSettings();

	/**
	 * 게임 종료
	 */
	UFUNCTION(BlueprintCallable, Category = "TitleViewModel")
	void EndGame();

	/**
	 * 해당 맵을 로드하면서 로딩 맵으로 이동
	 */
	UFUNCTION(BlueprintCallable, Category = "TitleViewModel")
	void MoveToLoadingMap();

public:
	/* 바인딩 함수 */

	/**
	 * SaveGameSubsystem에서 데이터 변동 시, 처리할 함수
	 * @param PlayerSaveList 변동된 SavePlayer 데이터들
	 */
	void UpdateTitleMenu(const TDoubleLinkedList<TObjectPtr<USavePlayer>>& PlayerSaveList);

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetState", Getter = "GetState", meta = (AllowPrivateAccess))
	ETitleState mState;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetHasPressedKey", Getter = "HasPressedKey", meta = (AllowPrivateAccess))
	bool bHasPressedKey;
};

