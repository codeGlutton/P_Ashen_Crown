#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/Widget/ComposableViewModelBase.h"

#include "P_Ashen_Crown/Component/InteractableTargetComponent.h"
#include "P_Ashen_Crown/Component/DialogueComponent.h"

#include "P_Ashen_Crown/Widget/DragDropWidgetBase.h"
#include "P_Ashen_Crown/Component/InventoryComponent.h"

#include "P_Ashen_Crown/Component/LockOnTargetComponent.h"

#include "P_Ashen_Crown/Widget/Main/PauseViewHandler.h"

#include "GameplayEffectTypes.h"

#include "MainViewModel.generated.h"

DECLARE_DELEGATE(FOnWorkJob);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectDialogueBranch, int32, Index);

/************************
	UInteractViewModel
*************************/

UENUM(BlueprintType)
enum class EInteractViewModelState : uint8
{
	Remove = 0 UMETA(ToolTip = "뷰포트 제거"),
	Open UMETA(ToolTip = "뷰포트 추가, 실행 애니메이션 실행"),
	Close UMETA(ToolTip = "종료 애니메이션 실행")
};

/**
 * 상호작용 가능을 표기하는 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UInteractViewModel : public UComposableViewModelBase, public IInteractableViewHandler
{
	GENERATED_BODY()

public:
	/* FieldNotify Getter/Setter */

	UFUNCTION(BlueprintCallable, Category = "InteractViewModel")
	void SetState(EInteractViewModelState State);
	EInteractViewModelState GetState() const;

	UFUNCTION(BlueprintCallable, Category = "InteractViewModel")
	void SetDisplayName(const FText& DisplayName);
	const FText& GetDisplayName() const;

public:
	/* FieldNotify 함수 */

	/**
	 * State Enum을 실수로 변환
	 * @return 변환된 값
	 */
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetFloatFromState() const;

public:
	/* 수동 실행 함수 */

	/**
	 * 특정 위젯 작업 이후에 예약된 작업 실행
	 */
	UFUNCTION(BlueprintCallable, Category = "InteractViewModel")
	void PopJobQueue();

	/**
	 * 인터렉션 시도
	 */
	UFUNCTION(BlueprintCallable, Category = "InteractViewModel")
	void Interact();

public:
	/* IInteractableViewHandler 인터페이스 */

	virtual void ShowView(UInteractableTargetComponent* const TargetComp) override;
	virtual void SetDisplayText(const FText& DisplayText) override;
	virtual void CloseView() override;

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetState", Getter = "GetState", BlueprintSetter = "SetState", meta = (AllowPrivateAccess))
	EInteractViewModelState mState;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetDisplayName", Getter = "GetDisplayName", BlueprintSetter = "SetDisplayName", meta = (AllowPrivateAccess))
	FText mDisplayName;

private:
	UInteractableTargetComponent* mTargetComp;
	TQueue<FOnWorkJob> mJobQueue;
};

/************************
 UDialogueSelectViewModel
*************************/

/**
 * NPC 등의 대화창 선택 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UDialogueSelectViewModel : public UComposableViewModelBase
{
	GENERATED_BODY()

public:
	/* FieldNotify Getter/Setter */

	UFUNCTION(BlueprintCallable, Category = "DialogueSelectViewModel")
	void SetSelectionName(const FText& SelectionName);
	const FText& GetSelectionName() const;

	UFUNCTION(BlueprintCallable, Category = "DialogueSelectViewModel")
	void SetEnabled(bool bWillbeEnabled);
	bool IsEnabled() const;

	UFUNCTION(BlueprintCallable, Category = "DialogueSelectViewModel")
	void SetSelectionIndex(int32 SelectionIndex);
	int32 GetSelectionIndex() const;

public:
	/* 수동 실행 함수 */

	/**
	 * 해당 브랜치 선택 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogueSelectViewModel")
	void Select();

public:
	UPROPERTY(BlueprintAssignable, Category = "DialogueSelectViewModel")
	FOnSelectDialogueBranch OnSelectDialogueBranch;

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetSelectionName", Getter = "GetSelectionName", BlueprintSetter = "SetSelectionName", meta = (AllowPrivateAccess))
	FText mSelectionName;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetEnabled", Getter = "IsEnabled", BlueprintSetter = "SetEnabled", meta = (AllowPrivateAccess))
	bool bIsEnabled;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetSelectionIndex", Getter = "GetSelectionIndex", BlueprintSetter = "SetSelectionIndex", meta = (AllowPrivateAccess))
	int32 mSelectionIndex;
};

/************************
	UDialogueViewModel
*************************/

UENUM(BlueprintType)
enum class EDialogueViewModelState : uint8
{
	Remove = 0 UMETA(ToolTip = "뷰포트 제거"),
	Open UMETA(ToolTip = "뷰포트 추가, 실행 애니메이션 실행"),
	Erase UMETA(ToolTip = "텍스트 제거 애니메이션 실행"),
	Write UMETA(ToolTip = "텍스트 출력 애니메이션 실행"),
	Close UMETA(ToolTip = "종료 애니메이션 실행")
};

/**
 * NPC 등의 대화창 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UDialogueViewModel : public UComposableViewModelBase, public IDialogueViewHandler
{
	GENERATED_BODY()

public:
	/* FieldNotify Getter/Setter */

	UFUNCTION(BlueprintCallable, Category = "DialogueViewModel")
	void SetState(EDialogueViewModelState State);
	EDialogueViewModelState GetState() const;

	UFUNCTION(BlueprintCallable, Category = "DialogueViewModel")
	void SetSpeakerName(const FText& SpeakerName);
	const FText& GetSpeakerName() const;

	UFUNCTION(BlueprintCallable, Category = "DialogueViewModel")
	void SetCurDialogue(const FText& CurDialogue);
	const FText& GetCurDialogue() const;

	UFUNCTION(BlueprintCallable, Category = "DialogueViewModel")
	void SetSelectable(bool bWillBeSelectable);
	bool IsSelectable() const;
	
public:
	/* FieldNotify 함수 */

	/**
	 * State Enum을 실수로 변환
	 * @return 변환된 값
	 */
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetFloatFromState() const;

public:
	/* 수동 실행 함수 */

	/**
	 * 특정 위젯 작업 이후에 예약된 작업 실행
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogueSelectViewModel")
	void PopJobQueue();

	/**
	 * 다음 페이지로 이동 (특정 위젯 작업까지 지연)
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogueSelectViewModel")
	void LazyTurnNextPage();

public:
	/* 바인딩 함수 */

	/**
	 * 선택된 브랜치 이동 (특정 위젯 작업까지 지연)
	 * @param Index 브랜치 인덱스
	 */
	UFUNCTION()
	void LazyEnterDialogueBranch(int32 Index);

	/**
	 * 다음 페이지로 이동
	 */
	void TurnNextPage();

	/**
	 * 선택된 브랜치 이동
	 */
	void EnterDialogueBranch(int32 Index);

	/**
	 * 뷰포트에서 제거
	 */
	void RemoveFromViewport();

public:
	/* IDialogueViewHandler 인터페이스 */

	virtual void ShowView(UDialogueComponent* const DialogueComp) override;
	virtual void SetSpeakerText(const FText& Speaker) override;
	virtual void SetPageText(const FText& Contents) override;
	virtual void SetChoiceTexts(const TArray<FDialogueChoiceViewInfo>& ChoiceInfos) override;
	virtual void CloseView() override;

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetState", Getter = "GetState", BlueprintSetter = "SetState", meta = (AllowPrivateAccess))
	EDialogueViewModelState mState;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetSpeakerName", Getter = "GetSpeakerName", BlueprintSetter = "SetSpeakerName", meta = (AllowPrivateAccess))
	FText mSpeakerName;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetCurDialogue", Getter = "GetCurDialogue", BlueprintSetter = "SetCurDialogue", meta = (AllowPrivateAccess))
	FText mCurDialogue;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetSelectable", Getter = "IsSelectable", BlueprintSetter = "SetSelectable", meta = (AllowPrivateAccess))
	bool bIsSelectable;

	UPROPERTY(BlueprintReadWrite, FieldNotify, meta = (AllowPrivateAccess))
	TArray<TObjectPtr<UDialogueSelectViewModel>> mSelectVMs;

private:
	UDialogueComponent* mDialogueComp;
	TQueue<FOnWorkJob> mJobQueue;
};

/************************
	 ULockOnViewModel
*************************/

/**
 * 락온 대상을 표기하는 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API ULockOnViewModel : public UComposableViewModelBase, public ILockOnViewHandler
{
	GENERATED_BODY()

public:
	/* FieldNotify Getter/Setter */

	UFUNCTION(BlueprintCallable, Category = "InteractViewModel")
	void SetVisibility(bool bWillBeVisibility);
	bool GetVisibility() const;

public:
	/* FieldNotify 함수 */

	UFUNCTION(BlueprintPure, FieldNotify)
	ESlateVisibility GetSlateVisibility() const;

public:
	/* IInteractableViewHandler 인터페이스 */

	virtual void ShowView(ULockOnTargetComponent* const TargetComp) override;
	virtual void CloseView() override;

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetVisibility", Getter = "GetVisibility", BlueprintSetter = "SetVisibility", meta = (AllowPrivateAccess))
	bool bIsVisibility;
};

/************************
UInventoryFilterViewModel
*************************/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickFilter);

/**
 * 메인 UI의 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UInventoryFilterViewModel : public UComposableViewModelBase
{
	GENERATED_BODY()
	
public:
	UInventoryFilterViewModel();

public:
	/* FieldNotify Getter/Setter */

	UFUNCTION(BlueprintCallable, Category = "InventoryFilterViewModel")
	void Select(bool bIsWillSelected);
	bool IsSelected() const;

public:
	/* 수동 실행 함수 */

	// 필터 여부 토글
	UFUNCTION(BlueprintCallable, Category = "InventoryFilterViewModel")
	void ToggleSelected();

public:
	// 필터 변경 시 호출될 대리자
	UPROPERTY(BlueprintAssignable, Category = "InventoryFilterViewModel")
	FOnClickFilter OnClickFilter;

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "Select", Getter = "IsSelected", BlueprintSetter = "Select", meta = (AllowPrivateAccess))
	bool bIsSelected;
};

/************************
 UInventorySlotViewModel
*************************/

UENUM(BlueprintType)
enum class EInventorySlotType : uint8
{
	InventorySlot = 0,
	EquipSlot,
};

DECLARE_DELEGATE_FourParams(FOnTryToSwapInventorySlot, int32 /* LhsSlotVMIndex */, EInventorySlotType /* LhsSlotType */, int32 /* RhsSlotVMIndex */, EInventorySlotType /* RhsSlotType */);

/**
 * 인벤토리 슬롯의 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UInventorySlotViewModel : public UComposableViewModelBase, public IDragDropViewHandler
{
	GENERATED_BODY()

public:
	UInventorySlotViewModel();

public:
	/* FieldNotify Getter/Setter */

	UFUNCTION(BlueprintCallable, Category = "InventorySlotViewModel")
	void SetEnabled(bool bWillbeEnabled);
	bool IsEnabled() const;

public:
	void SetSlotType(EInventorySlotType SlotType);
	EInventorySlotType GetSlotType() const;

	void SetSlotIndex(int32 SlotIndex);
	int32 GetSlotIndex() const;

	void SetItemInfo(TSharedPtr<const FGameItemInfo> ItemInfo);

public:
	/* FieldNotify 함수 */

	UFUNCTION(BlueprintPure, FieldNotify)
	EItemType GetItemType() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	UTexture2D* GetItemIcon() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	const FText& GetItemName() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	const FText& GetItemDesc() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	bool CanDrag() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	const FText& GetAbilityText() const;

public:
	void SetToolTipWidget(UUserWidget* ToolTipWidget);
	UFUNCTION(BlueprintCallable, Category = "InventorySlotViewModel")
	UUserWidget* GetToolTipWidget() const;

	/**
	 * 툴팁 정보를 현 Slot으로 갱신하는 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "InventorySlotViewModel")
	void UpdateToolTip();

public:
	/* IDragDropViewHandler 인터페이스 */

	virtual void OnBeginDrag() override;
	virtual void OnEndDrag() override;
	virtual void OnDrop(IDragDropViewHandler* DragHandler) override;

public:
	FOnTryToSwapInventorySlot OnTryToSwapInventorySlot;

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetEnabled", Getter = "IsEnabled", BlueprintSetter = "SetEnabled", meta = (AllowPrivateAccess))
	bool bIsEnabled;

private:
	EInventorySlotType mSlotType;
	int32 mSlotIndex;
	TSharedPtr<const FGameItemInfo> mItemInfo;

private:
	TObjectPtr<UUserWidget> mToolTipWidget;
};

/************************
UInventoryTrashCanViewModel
*************************/

DECLARE_DELEGATE_TwoParams(FOnTryToClearInventorySlot, int32 /* SlotVMIndex */, EInventorySlotType /* SlotType */);

/**
 * 인벤토리 쓰레기통의 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UInventoryTrashCanViewModel : public UComposableViewModelBase, public IDragDropViewHandler
{
	GENERATED_BODY()

public:
	/* FieldNotify Getter/Setter */

public:
	/* IDragDropViewHandler 인터페이스 */

	virtual void OnBeginDrag() override {};
	virtual void OnEndDrag() override {};
	virtual void OnDrop(IDragDropViewHandler* DragHandler) override;

public:
	FOnTryToClearInventorySlot OnTryToClearInventorySlot;

private:
	int32 mSlotIndex;
};

/************************
   UInventoryViewModel
*************************/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeViewVisibility);

/**
 * 인벤토리 필터 bitMask용 Flag 타입
 */
UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EInventoryFilterType : uint8
{
	None = 0 UMETA(Hidden),
	Equip = 1 << 0,
	Potion = 1 << 1,
	Rune = 1 << 2,

	All = Equip + Potion + Rune
};
ENUM_CLASS_FLAGS(EInventoryFilterType);

/**
 * 인벤토리의 필터링된 결과를 저장하기 위한 구조체
 */
USTRUCT(BlueprintType)
struct FInventoryFilter
{
	GENERATED_BODY()

public:
	/**
	 * 필터에 포함되는지를 확인하고 가능할 경우, 삽입하는 함수
	 * @param SlotVM 삽입할 Inventory Slot ViewModel
	 * @return 삽입 여부
	 */
	bool TryAddSlotVM(UInventorySlotViewModel* SlotVM);
	/**
	 * 슬롯에 업데이트에 맞추어 필터 결과를 수정하는 함수 
	 * @param SlotVM 업데이트할 Inventory Slot ViewModel
	 * @param bWasChangedSlotCount Slot 갯수에 변동점이 있었는지 여부
	 */
	void UpdateSlotVM(UInventorySlotViewModel* SlotVM, bool& bWasChangedSlotCount);

protected:
	/**
	 * 아이템 타입이 해당 필터에 걸러지지않고 포함되는지 여부 확인 함수
	 * @param ItemType 아이템 타입
	 * @return 필터링으로 허용된 타입인지 여부
	 */
	bool IsItemTypeAllowed(EItemType ItemType);

public:
	UPROPERTY(VisibleAnywhere, meta = (Bitmask, BitmaskEnum = "EInventoryFilterType"))
	int32 mFilterFlag;
	TArray<UInventorySlotViewModel*> mSlotVMs;
};

/**
 * 인벤토리의 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UInventoryViewModel : public UComposableViewModelBase, public IInventoryViewHandler
{
	GENERATED_BODY()

public:
	UInventoryViewModel();

public:
	/* FieldNotify Getter/Setter */

	UFUNCTION(BlueprintCallable, Category = "InventoryViewModel")
	void SetVisiblity(bool bWillBeVisiblity);
	bool IsVisiblity() const;

	UFUNCTION(BlueprintCallable, Category = "InventoryViewModel")
	void SetMaxHP(float MaxHP);
	float GetMaxHP() const;

	UFUNCTION(BlueprintCallable, Category = "InventoryViewModel")
	void SetMaxStemina(float MaxStemina);
	float GetMaxStemina() const;

	UFUNCTION(BlueprintCallable, Category = "InventoryViewModel")
	void SetMoveSpeed(float MoveSpeed);
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "InventoryViewModel")
	void SetAttackDamage(float AttackDamage);
	float GetAttackDamage() const;

	UFUNCTION(BlueprintCallable, Category = "InventoryViewModel")
	void SetDefense(float Defense);
	float GetDefense() const;

public:
	/* FieldNotify 함수 */

	UFUNCTION(BlueprintPure, FieldNotify)
	TEnumAsByte<EUMGSequencePlayMode::Type> GetUMGSequencePlayMode() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	const TArray<UInventorySlotViewModel*>& GetCurInventorySlotVMs() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	UInventorySlotViewModel* GetWeaponSlotVM() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	UInventorySlotViewModel* GetArmorSlotVM() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	UInventoryTrashCanViewModel* GetTrashVM() const;

public:
	/* 수동 실행 함수 */

	/**
	 * 필터를 키고 끄는 함수
	 * @param FilterType 영향을 줄 필터 타입
	 * @param bSwitchOn 필터를 킬지 끌지 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "InventoryViewModel")
	void SwitchFilter(EInventoryFilterType FilterType, bool bSwitchOn);
	
	/**
	 * 아이템 교환 함수
	 * @param LhsSlotVMIndex 교환할 항목A 인덱스
	 * @param LhsSlotType 교환할 항목A 타입
	 * @param RhsSlotVMIndex 교환할 항목B 인덱스
	 * @param RhsSlotType 교환할 항목B 타입
	 */
	UFUNCTION(BlueprintCallable, Category = "InventoryViewModel")
	void SwapItemSlot(int32 LhsSlotVMIndex, EInventorySlotType LhsSlotType, int32 RhsSlotVMIndex, EInventorySlotType RhsSlotType);

	/**
	 * 아이템 제거 함수
	 * @param SlotVMIndex 제거할 항목 인덱스
	 * @param SlotType 제거할 항목 타입
	 */
	UFUNCTION(BlueprintCallable, Category = "InventoryViewModel")
	void ClearItemSlot(int32 SlotVMIndex, EInventorySlotType SlotType);

protected:
	/**
	 * 필터 캐싱 데이터 업데이트
	 */
	void UpdateFilterCaches();

public:
	/* IInventoryViewHandler 인터페이스 */

	virtual void ConnectToInventory(UInventoryComponent* const InventoryComponent) override;
	virtual void ShowView() override;
	virtual void CloseView() override;

public:
	/* 바인딩 함수 */

	/**
	 * 인벤토리 아이템 정보 업데이트를 받아, 갱신하는 함수
	 * @param SlotIndex 업데이트된 아이템 인덱스
	 * @param ItemInfo 업데이트된 아이템 정보
	 * @param ItemInfo 이전 아이템 정보
	 */
	void UpdateInventoryItemSlot(int32 SlotIndex, TSharedPtr<const FGameItemInfo> ItemInfo, TSharedPtr<const FGameItemInfo> PreItemInfo);
	/**
	 * 장비 아이템 정보 업데이트를 받아, 갱신하는 함수
	 * @param SlotIndex 업데이트된 아이템 인덱스
	 * @param ItemInfo 업데이트된 아이템 정보
	 * @param PreItemInfo 이전 아이템 정보
	 */
	void UpdateEquipItemSlot(int32 SlotIndex, TSharedPtr<const FGameItemInfo> ItemInfo, TSharedPtr<const FGameItemInfo> PreItemInfo);

protected:
	void UpdateHP();
	void UpdateStemina();
	void UpdateMoveSpeed();
	void UpdateAttackDamage();
	void UpdateDefense();

public:
	UPROPERTY(BlueprintAssignable, Category = "InventoryViewModel")
	FOnChangeViewVisibility OnChangeViewVisibility;

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetVisiblity", Getter = "IsVisiblity", BlueprintSetter = "SetVisiblity", meta = (AllowPrivateAccess))
	bool bIsVisiblity;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetMaxHP", Getter = "GetMaxHP", BlueprintSetter = "SetMaxHP", meta = (AllowPrivateAccess))
	float mMaxHP;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetMaxStemina", Getter = "GetMaxStemina", BlueprintSetter = "SetMaxStemina", meta = (AllowPrivateAccess))
	float mMaxStemina;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetMoveSpeed", Getter = "GetMoveSpeed", BlueprintSetter = "SetMoveSpeed", meta = (AllowPrivateAccess))
	float mMoveSpeed;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetAttackDamage", Getter = "GetAttackDamage", BlueprintSetter = "SetAttackDamage", meta = (AllowPrivateAccess))
	float mAttackDamage;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetDefense", Getter = "GetDefense", BlueprintSetter = "SetDefense", meta = (AllowPrivateAccess))
	float mDefense;

private:
	// 모든 Slot ViewModel 원본
	UPROPERTY()
	TArray<TObjectPtr<UInventorySlotViewModel>> mAllInventorySlotVMs;
	UPROPERTY()
	TArray<TObjectPtr<UInventorySlotViewModel>> mAllEquipSlotVMs;
	UPROPERTY()
	TObjectPtr<UInventoryTrashCanViewModel> mTrashVM;

	UPROPERTY(VisibleAnywhere, meta = (Bitmask, BitmaskEnum = "EInventoryFilterType"))
	int32 mCurFilterFlag;

private:
	// 최대 캐싱 양
	static const int32 mMaxFilterCacheCount = 5;

	// 필터링을 위해 최근 사용한 필터 결과값 N개 캐싱 (LRU)
	TDoubleLinkedList<FInventoryFilter> mFilterCaches;
	bool bIsFilterCacheDirty;

private:
	TObjectPtr<UInventoryComponent> mInventoryComp;
};

/************************
	  UPauseViewModel
*************************/

/**
 * Pause UI의 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UPauseViewModel : public UComposableViewModelBase, public IPauseViewHandler
{
	GENERATED_BODY()

public:
	/* FieldNotify Getter/Setter */

	UFUNCTION(BlueprintCallable, Category = "PauseViewModel")
	void SetVisiblity(bool bWillBeVisiblity);
	bool IsVisiblity() const;

	UFUNCTION(BlueprintCallable, Category = "PauseViewModel")
	void SetStageName(const FText& StageName);
	const FText& GetStageName() const;

	UFUNCTION(BlueprintCallable, Category = "PauseViewModel")
	void SetLevel(int32 Level);
	int32 GetLevel() const;

	UFUNCTION(BlueprintCallable, Category = "PauseViewModel")
	void SetSaveDate(FDateTime SaveDate);
	FDateTime GetSaveDate() const;

public:
	/* FieldNotify 함수 */

	UFUNCTION(BlueprintPure, FieldNotify)
	TEnumAsByte<EUMGSequencePlayMode::Type> GetUMGSequencePlayMode() const;

public:
	/* 수동 실행 함수 */

	/**
	 * 다시 게임으로 복귀
	 */
	UFUNCTION(BlueprintCallable, Category = "PauseViewModel")
	void ContinueGame();

	/**
	 * 게임에 대한 값을 설정
	 */
	UFUNCTION(BlueprintCallable, Category = "PauseViewModel")
	void ConfigSettings();

	/**
	 * 해당 맵을 로드하면서 로딩 맵으로 이동
	 */
	UFUNCTION(BlueprintCallable, Category = "PauseViewModel")
	void MoveToTitleMap();

public:
	/* IPauseViewHandler 인터페이스 */

	virtual void ToggleView() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "PauseViewModel")
	FOnChangeViewVisibility OnChangeViewVisibility;

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetVisiblity", Getter = "IsVisiblity", BlueprintSetter = "SetVisiblity", meta = (AllowPrivateAccess))
	bool bIsVisiblity;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetStageName", Getter = "GetStageName", BlueprintSetter = "SetStageName", meta = (AllowPrivateAccess))
	FText mStageName;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetLevel", Getter = "GetLevel", BlueprintSetter = "SetLevel", meta = (AllowPrivateAccess))
	int32 mLevel;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetSaveDate", Getter = "GetSaveDate", BlueprintSetter = "SetSaveDate", meta = (AllowPrivateAccess))
	FDateTime mSaveDate;
};

/************************
  UMonsterHPBarViewModel
*************************/

/**
 * 몬스터 HP UI의 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UMonsterHPBarViewModel : public UComposableViewModelBase
{
	GENERATED_BODY()

public:
	/* FieldNotify Getter/Setter */

	UFUNCTION(BlueprintCallable, Category = "HUDViewModel")
	void SetHpPercent(float HpPercent);
	float GetHpPercent() const;

	UFUNCTION(BlueprintCallable, Category = "HUDViewModel")
	void SetDisplayName(const FText& DisplayName);
	const FText& GetDisplayName() const;

public:
	virtual void Initialize_Implementation() override;

protected:
	void UpdateHP();

public:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetHpPercent", Getter = "GetHpPercent", BlueprintSetter = "SetHpPercent", meta = (AllowPrivateAccess))
	float mHpPercent;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetDisplayName", Getter = "GetDisplayName", BlueprintSetter = "SetDisplayName", meta = (AllowPrivateAccess))
	FText mDisplayName;
};

/************************
	  UHUDViewModel
*************************/

/**
 * 메인 HUD UI의 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UHUDViewModel : public UComposableViewModelBase
{
	GENERATED_BODY()
	
public:
	/* FieldNotify Getter/Setter */

	UFUNCTION(BlueprintCallable, Category = "HUDViewModel")
	void SetHpPercent(float HpPercent);
	float GetHpPercent() const;

	UFUNCTION(BlueprintCallable, Category = "HUDViewModel")
	void SetSteminaPercent(float SteminaPercent);
	float GetSteminaPercent() const;

	UFUNCTION(BlueprintCallable, Category = "HUDViewModel")
	void SetPotionCount(int32 PotionCount);
	int32 GetPotionCount() const;

	UFUNCTION(BlueprintCallable, Category = "HUDViewModel")
	void SetDisplayName(const FText& DisplayName);
	const FText& GetDisplayName() const;

	UFUNCTION(BlueprintCallable, Category = "HUDViewModel")
	void SetCanSkill(bool CanSkill);
	bool GetCanSkill() const;

	UFUNCTION(BlueprintCallable, Category = "HUDViewModel")
	void SetCanRoll(bool CanRoll);
	bool GetCanRoll() const;

public:
	virtual void Initialize_Implementation() override;

protected:
	/* 대리자로 호출될 업데이트 함수들 */

	void UpdateHP();
	void UpdateStemina();
	void UpdatePotionCount(int32 InventoryIndex, TSharedPtr<const FGameItemInfo> ItemInfo, TSharedPtr<const FGameItemInfo> PreItemInfo);

	void UpdateSkill();
	void UpdateRoll();

public:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetHpPercent", Getter = "GetHpPercent", BlueprintSetter = "SetHpPercent", meta = (AllowPrivateAccess))
	float mHpPercent;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetSteminaPercent", Getter = "GetSteminaPercent", BlueprintSetter = "SetSteminaPercent", meta = (AllowPrivateAccess))
	float mSteminaPercent;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetPotionCount", Getter = "GetPotionCount", BlueprintSetter = "SetPotionCount", meta = (AllowPrivateAccess))
	int32 mPotionCount;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetDisplayName", Getter = "GetDisplayName", BlueprintSetter = "SetDisplayName", meta = (AllowPrivateAccess))
	FText mDisplayName;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetCanSkill", Getter = "GetCanSkill", BlueprintSetter = "SetCanSkill", meta = (AllowPrivateAccess))
	bool bCanSkill;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetCanRoll", Getter = "GetCanRoll", BlueprintSetter = "SetCanRoll", meta = (AllowPrivateAccess))
	bool bCanRoll;
};

/************************
	UGameOverViewModel
*************************/

/**
 * 게임 오버 UI의 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UGameOverViewModel : public UComposableViewModelBase
{
	GENERATED_BODY()

public:
	virtual void Initialize_Implementation() override;

public:
	/**
	 * 해당 맵을 로드하면서 로딩 맵으로 이동
	 */
	UFUNCTION(BlueprintCallable, Category = "GameOverViewModel")
	void MoveToTitleMap();
};