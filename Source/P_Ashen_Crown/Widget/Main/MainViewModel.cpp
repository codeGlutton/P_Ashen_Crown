#include "MainViewModel.h"

#include "Animation/WidgetAnimation.h"
#include "Blueprint/UserWidget.h"
#include "View/MVVMView.h"

#include "P_Ashen_Crown/Game/Main/MainPlayerController.h"

#include "P_Ashen_Crown/Singleton/Subsystem/SaveGameSubsystem.h"
#include "P_Ashen_Crown/Setting/ACWorldSettings.h"
#include "P_Ashen_Crown/TableRow/StageData.h"
#include "P_Ashen_Crown/SaveGame/SavePlayer.h"

#include "P_Ashen_Crown/GAS/BaseAttributeSet.h"
#include "P_Ashen_Crown/GAS/AttributeSet/PlayerAttributeSet.h"
#include "P_Ashen_Crown/Character/Player/PlayerCharacter.h"

/************************
	UInteractViewModel
*************************/

void UInteractViewModel::SetState(EInteractViewModelState State)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mState, State))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetFloatFromState);
	}
}

EInteractViewModelState UInteractViewModel::GetState() const
{
	return mState;
}

void UInteractViewModel::SetDisplayName(const FText& DisplayName)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mDisplayName, DisplayName))
	{
	}
}

const FText& UInteractViewModel::GetDisplayName() const
{
	return mDisplayName;
}

float UInteractViewModel::GetFloatFromState() const
{
	return (float)mState;
}

void UInteractViewModel::PopJobQueue()
{
	if (mJobQueue.IsEmpty() == true)
	{
		return;
	}

	FOnWorkJob Job;
	mJobQueue.Dequeue(Job);
	Job.ExecuteIfBound();
}

void UInteractViewModel::Interact()
{
	if (IsValid(mTargetComp) == true)
	{
		mTargetComp->Interact();
	}
}

void UInteractViewModel::ShowView(UInteractableTargetComponent* const TargetComp)
{
	mJobQueue.Empty();
	mTargetComp = TargetComp;
	SetState(EInteractViewModelState::Open);
}

void UInteractViewModel::SetDisplayText(const FText& DisplayText)
{
	SetDisplayName(DisplayText);
}

void UInteractViewModel::CloseView()
{
	mJobQueue.Enqueue(FOnWorkJob::CreateLambda([this]() {
		SetState(EInteractViewModelState::Remove);
		}));
	SetState(EInteractViewModelState::Close);
}

/************************
 UDialogueSelectViewModel
*************************/

void UDialogueSelectViewModel::SetSelectionName(const FText& SelectionName)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mSelectionName, SelectionName))
	{
	}
}

const FText& UDialogueSelectViewModel::GetSelectionName() const
{
	return mSelectionName;
}

void UDialogueSelectViewModel::SetEnabled(bool bWillbeEnabled)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(bIsEnabled, bWillbeEnabled))
	{
	}
}

bool UDialogueSelectViewModel::IsEnabled() const
{
	return bIsEnabled;
}

void UDialogueSelectViewModel::SetSelectionIndex(int32 SelectionIndex)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mSelectionIndex, SelectionIndex))
	{
	}
}

int32 UDialogueSelectViewModel::GetSelectionIndex() const
{
	return mSelectionIndex;
}

void UDialogueSelectViewModel::Select()
{
	OnSelectDialogueBranch.Broadcast(mSelectionIndex);
}

/************************
	UDialogueViewModel
*************************/

void UDialogueViewModel::SetState(EDialogueViewModelState State)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mState, State))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetFloatFromState);
	}
}

EDialogueViewModelState UDialogueViewModel::GetState() const
{
	return mState;
}

void UDialogueViewModel::SetSpeakerName(const FText& SpeakerName)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mSpeakerName, SpeakerName))
	{
	}
}

const FText& UDialogueViewModel::GetSpeakerName() const
{
	return mSpeakerName;
}

void UDialogueViewModel::SetCurDialogue(const FText& CurDialogue)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mCurDialogue, CurDialogue))
	{
	}
}

const FText& UDialogueViewModel::GetCurDialogue() const
{
	return mCurDialogue;
}

void UDialogueViewModel::SetSelectable(bool bWillBeSelectable)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(bIsSelectable, bWillBeSelectable))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(mSelectVMs);
	}
}

bool UDialogueViewModel::IsSelectable() const
{
	return bIsSelectable;
}

float UDialogueViewModel::GetFloatFromState() const
{
	return (float)mState;
}

void UDialogueViewModel::PopJobQueue()
{
	if (mJobQueue.IsEmpty() == true)
	{
		return;
	}

	FOnWorkJob Job;
	mJobQueue.Dequeue(Job);
	Job.ExecuteIfBound();
}

void UDialogueViewModel::LazyTurnNextPage()
{
	mJobQueue.Enqueue(FOnWorkJob::CreateLambda([this]()
		{
			TurnNextPage();
		}));
	SetState(EDialogueViewModelState::Erase);
}

void UDialogueViewModel::LazyEnterDialogueBranch(int32 Index)
{
	mJobQueue.Enqueue(FOnWorkJob::CreateLambda([this, Index]()
		{
			EnterDialogueBranch(Index);
		}));
	SetState(EDialogueViewModelState::Erase);
}

void UDialogueViewModel::TurnNextPage()
{
	mDialogueComp->TurnNextPage();
}

void UDialogueViewModel::EnterDialogueBranch(int32 Index)
{
	mJobQueue.Empty();
	mSelectVMs.Empty();
	SetSelectable(false);
	mDialogueComp->DecideOnChoice(Index);
}

void UDialogueViewModel::RemoveFromViewport()
{
	SetState(EDialogueViewModelState::Remove);
}

void UDialogueViewModel::ShowView(UDialogueComponent* const DialogueComp)
{
	mDialogueComp = DialogueComp;
	SetState(EDialogueViewModelState::Open);
	auto* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();
	if (IsValid(MainPlayerController) == true)
	{
		MainPlayerController->IncreaseUIOnlyModeCount();
	}
}

void UDialogueViewModel::SetSpeakerText(const FText& Speaker)
{
	SetSpeakerName(Speaker);
	SetState(EDialogueViewModelState::Write);
}

void UDialogueViewModel::SetPageText(const FText& Contents)
{
	SetCurDialogue(Contents);
}

void UDialogueViewModel::SetChoiceTexts(const TArray<FDialogueChoiceViewInfo>& ChoiceInfos)
{
	mSelectVMs.Empty();

	const int32 Size = ChoiceInfos.Num();
	for (int32 i = 0; i < Size; ++i)
	{
		auto* NewVM = NewObject<UDialogueSelectViewModel>(this);
		mSelectVMs.Add(NewVM);

		NewVM->SetSelectionName(ChoiceInfos[i].mChoiceText);
		NewVM->SetEnabled(ChoiceInfos[i].bIsEnabled);
		NewVM->SetSelectionIndex(i);
		NewVM->OnSelectDialogueBranch.AddDynamic(this, &UDialogueViewModel::LazyEnterDialogueBranch);
	}
	SetSelectable(true);
}

void UDialogueViewModel::CloseView()
{
	mDialogueComp = nullptr;

	SetSpeakerName(FText::GetEmpty());
	SetCurDialogue(FText::GetEmpty());

	mJobQueue.Enqueue(FOnWorkJob::CreateLambda([this]()
		{
			auto* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();
			if (IsValid(MainPlayerController) == true)
			{
				MainPlayerController->DecreaseUIOnlyModeCount();
			}
			RemoveFromViewport();
		}));
	SetState(EDialogueViewModelState::Close);
}

/************************
	 ULockOnViewModel
*************************/

void ULockOnViewModel::SetVisibility(bool bWillBeVisibility)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(bIsVisibility, bWillBeVisibility))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetSlateVisibility);
	}
}

bool ULockOnViewModel::GetVisibility() const
{
	return bIsVisibility;
}

ESlateVisibility ULockOnViewModel::GetSlateVisibility() const
{
	return bIsVisibility == true ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
}

void ULockOnViewModel::ShowView(ULockOnTargetComponent* const TargetComp)
{
	SetVisibility(true);
}

void ULockOnViewModel::CloseView()
{
	SetVisibility(false);
}

/************************
UInventoryFilterViewModel
*************************/

UInventoryFilterViewModel::UInventoryFilterViewModel() :
	bIsSelected(true)
{
}

void UInventoryFilterViewModel::Select(bool bIsWillSelected)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(bIsSelected, bIsWillSelected))
	{
		OnClickFilter.Broadcast();
	}
}

bool UInventoryFilterViewModel::IsSelected() const
{
	return bIsSelected;
}

void UInventoryFilterViewModel::ToggleSelected()
{
	Select(!bIsSelected);
}

/************************
 UInventorySlotViewModel
*************************/

UInventorySlotViewModel::UInventorySlotViewModel() :
	bIsEnabled(true),
	mSlotType(EInventorySlotType::InventorySlot),
	mSlotIndex(),
	mItemInfo(nullptr)
{
}

void UInventorySlotViewModel::SetEnabled(bool bWillbeEnabled)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(bIsEnabled, bWillbeEnabled))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CanDrag);
	}
}

bool UInventorySlotViewModel::IsEnabled() const
{
	return false;
}

void UInventorySlotViewModel::SetSlotType(EInventorySlotType SlotType)
{
	mSlotType = SlotType;
}

EInventorySlotType UInventorySlotViewModel::GetSlotType() const
{
	return mSlotType;
}

void UInventorySlotViewModel::SetSlotIndex(int32 SlotIndex)
{
	mSlotIndex = SlotIndex;
}

int32 UInventorySlotViewModel::GetSlotIndex() const
{
	return mSlotIndex;
}

void UInventorySlotViewModel::SetItemInfo(TSharedPtr<const FGameItemInfo> ItemInfo)
{
	if (mItemInfo != ItemInfo)
	{
		mItemInfo = ItemInfo;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetItemType);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetItemIcon);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetItemName);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetItemDesc);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CanDrag);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAbilityText);
	}
}

EItemType UInventorySlotViewModel::GetItemType() const
{
	return mItemInfo == nullptr ? EItemType::None : mItemInfo->mType;
}

UTexture2D* UInventorySlotViewModel::GetItemIcon() const
{
	return mItemInfo == nullptr ? nullptr : mItemInfo->mTexture;
}

const FText& UInventorySlotViewModel::GetItemName() const
{
	return mItemInfo == nullptr ? FText::GetEmpty() : mItemInfo->mName;
}

const FText& UInventorySlotViewModel::GetItemDesc() const
{
	return mItemInfo == nullptr ? FText::GetEmpty() : mItemInfo->mDesc;
}

bool UInventorySlotViewModel::CanDrag() const
{
	return bIsEnabled == true && mItemInfo != nullptr;
}

const FText& UInventorySlotViewModel::GetAbilityText() const
{
	return mItemInfo == nullptr ? FText::GetEmpty() : mItemInfo->GetAbilityText();
}

void UInventorySlotViewModel::SetToolTipWidget(UUserWidget* ToolTipWidget)
{
	mToolTipWidget = ToolTipWidget;
}

UUserWidget* UInventorySlotViewModel::GetToolTipWidget() const
{
	return mToolTipWidget;
}

void UInventorySlotViewModel::UpdateToolTip()
{
	auto* ToolTipView = Cast<UMVVMView>(mToolTipWidget->GetExtension(UMVVMView::StaticClass()));
	if (IsValid(ToolTipView) == false)
	{
		return;
	}
	ToolTipView->SetViewModel(TEXT("InventorySlotViewModel"), this);
}

void UInventorySlotViewModel::OnBeginDrag()
{
	SetEnabled(false);
}

void UInventorySlotViewModel::OnEndDrag()
{
	SetEnabled(true);
}

void UInventorySlotViewModel::OnDrop(IDragDropViewHandler* DragHandler)
{
	UInventorySlotViewModel* SlotVM = Cast<UInventorySlotViewModel>(DragHandler);
	OnTryToSwapInventorySlot.ExecuteIfBound(mSlotIndex, mSlotType, SlotVM->GetSlotIndex(), SlotVM->GetSlotType());
}

/************************
UInventoryTrashCanViewModel
*************************/

void UInventoryTrashCanViewModel::OnDrop(IDragDropViewHandler* DragHandler)
{
	UInventorySlotViewModel* SlotVM = Cast<UInventorySlotViewModel>(DragHandler);
	OnTryToClearInventorySlot.ExecuteIfBound(SlotVM->GetSlotIndex(), SlotVM->GetSlotType());
}

/************************
   UInventoryViewModel
*************************/

bool FInventoryFilter::TryAddSlotVM(UInventorySlotViewModel* SlotVM)
{
	// 현재 필터에 포함되지 않는 경우
	if (IsItemTypeAllowed(SlotVM->GetItemType()) == false)
	{
		return false;
	}

	mSlotVMs.Add(SlotVM);
	return true;
}

void FInventoryFilter::UpdateSlotVM(UInventorySlotViewModel* SlotVM, bool& bWasChangedSlotCount)
{
	// 변경된 슬롯이 필터에서 허용하는 타입인지
	bool bIsAllowed = IsItemTypeAllowed(SlotVM->GetItemType());
	// 변경된 슬롯이 이전에 포함되었는지
	bool bIsFound = false;
	int32 TargetIndex;
	for (TargetIndex = 0; TargetIndex < mSlotVMs.Num(); ++TargetIndex)
	{
		if (mSlotVMs[TargetIndex]->GetSlotIndex() == SlotVM->GetSlotIndex())
		{
			bIsFound = true;
			break;
		}
		else if (mSlotVMs[TargetIndex]->GetSlotIndex() > SlotVM->GetSlotIndex())
		{
			break;
		}
	}

	bWasChangedSlotCount = false;
	if (bIsFound != bIsAllowed)
	{
		if (bIsAllowed == true)
		{
			mSlotVMs.Insert(SlotVM, TargetIndex);
			bWasChangedSlotCount = true;
		}
		else
		{
			mSlotVMs.RemoveAt(TargetIndex);
			bWasChangedSlotCount = true;
		}
	}
}

bool FInventoryFilter::IsItemTypeAllowed(EItemType ItemType)
{
	if (EnumHasAllFlags(StaticCast<EInventoryFilterType>(mFilterFlag), EInventoryFilterType::All) == true)
	{
		return true;
	}

	EInventoryFilterType ItemFilterType;
	switch (ItemType)
	{
	case EItemType::Equip:
		ItemFilterType = EInventoryFilterType::Equip;
		break;
	case EItemType::Potion:
		ItemFilterType = EInventoryFilterType::Potion;
		break;
	case EItemType::Rune:
		ItemFilterType = EInventoryFilterType::Rune;
		break;
	default:
		return false;
	}

	return EnumHasAnyFlags(ItemFilterType, StaticCast<EInventoryFilterType>(mFilterFlag));
}

UInventoryViewModel::UInventoryViewModel() :
	bIsVisiblity(false),
	mAllInventorySlotVMs(),
	mCurFilterFlag(StaticCast<int32>(EInventoryFilterType::All)),
	mFilterCaches()
{
}

void UInventoryViewModel::SetVisiblity(bool bWillBeVisiblity)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(bIsVisiblity, bWillBeVisiblity))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetUMGSequencePlayMode);
		auto* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();
		if (IsValid(MainPlayerController) == true)
		{
			if (bIsVisiblity == true)
			{
				MainPlayerController->IncreaseUIOnlyModeCount();
			}
			else
			{
				MainPlayerController->DecreaseUIOnlyModeCount();
			}
		}
	}
}

bool UInventoryViewModel::IsVisiblity() const
{
	return bIsVisiblity;
}

void UInventoryViewModel::SetMaxHP(float MaxHP)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mMaxHP, MaxHP))
	{
	}
}

float UInventoryViewModel::GetMaxHP() const
{
	return mMaxHP;
}

void UInventoryViewModel::SetMaxStemina(float MaxStemina)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mMaxStemina, MaxStemina))
	{
	}
}

float UInventoryViewModel::GetMaxStemina() const
{
	return mMaxStemina;
}

void UInventoryViewModel::SetMoveSpeed(float MoveSpeed)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mMoveSpeed, MoveSpeed))
	{
	}
}

float UInventoryViewModel::GetMoveSpeed() const
{
	return mMoveSpeed;
}

void UInventoryViewModel::SetAttackDamage(float AttackDamage)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mAttackDamage, AttackDamage))
	{
	}
}

float UInventoryViewModel::GetAttackDamage() const
{
	return mAttackDamage;
}

void UInventoryViewModel::SetDefense(float Defense)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mDefense, Defense))
	{
	}
}

float UInventoryViewModel::GetDefense() const
{
	return mDefense;
}

TEnumAsByte<EUMGSequencePlayMode::Type> UInventoryViewModel::GetUMGSequencePlayMode() const
{
	return bIsVisiblity == true ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse;
}

const TArray<UInventorySlotViewModel*>& UInventoryViewModel::GetCurInventorySlotVMs() const
{
	if (mFilterCaches.IsEmpty() == true)
	{
		return mAllInventorySlotVMs;
	}
	auto& CurFilter = mFilterCaches.GetHead()->GetValue();
	return CurFilter.mSlotVMs;
}

UInventorySlotViewModel* UInventoryViewModel::GetWeaponSlotVM() const
{
	int32 WeaponIndex = StaticCast<int32>(EEquipType::Weapon);
	if (mAllEquipSlotVMs.Num() < WeaponIndex)
	{
		return nullptr;
	}
	return mAllEquipSlotVMs[WeaponIndex];
}

UInventorySlotViewModel* UInventoryViewModel::GetArmorSlotVM() const
{
	int32 ArmorIndex = StaticCast<int32>(EEquipType::Armor);
	if (mAllEquipSlotVMs.Num() < ArmorIndex)
	{
		return nullptr;
	}
	return mAllEquipSlotVMs[ArmorIndex];
}

UInventoryTrashCanViewModel* UInventoryViewModel::GetTrashVM() const
{
	return mTrashVM;
}

void UInventoryViewModel::SwitchFilter(EInventoryFilterType FilterType, bool bSwitchOn)
{
	EInventoryFilterType NextFilterFlag = StaticCast<EInventoryFilterType>(mCurFilterFlag);
	if (bSwitchOn == true)
	{
		EnumAddFlags(NextFilterFlag, FilterType);
	}
	else
	{
		EnumRemoveFlags(NextFilterFlag, FilterType);
	}
	mCurFilterFlag = StaticCast<int32>(NextFilterFlag);
	UpdateFilterCaches();
}

void UInventoryViewModel::SwapItemSlot(int32 LhsSlotVMIndex, EInventorySlotType LhsSlotType, int32 RhsSlotVMIndex, EInventorySlotType RhsSlotType)
{
	// 장비 간의 교환은 논리적으로 불가
	if (LhsSlotType == EInventorySlotType::EquipSlot && RhsSlotType == EInventorySlotType::EquipSlot)
	{
		return;
	}

	// 인벤토리 간의 교환
	if (LhsSlotType == RhsSlotType)
	{
		mInventoryComp->SwapItems(LhsSlotVMIndex, RhsSlotVMIndex);
	}
	// 장비 장착 및 해제
	else if (LhsSlotType == EInventorySlotType::EquipSlot)
	{
		mInventoryComp->EquipItem(LhsSlotVMIndex, RhsSlotVMIndex);
	}
	else
	{
		mInventoryComp->EquipItem(RhsSlotVMIndex, LhsSlotVMIndex);
	}
}

void UInventoryViewModel::ClearItemSlot(int32 SlotVMIndex, EInventorySlotType SlotType)
{
	if (SlotType == EInventorySlotType::InventorySlot)
	{
		mInventoryComp->RemoveInventoryItem(SlotVMIndex);
	}
	else
	{
		mInventoryComp->RemoveEquipItem(SlotVMIndex);
	}
}

void UInventoryViewModel::UpdateFilterCaches()
{
	/* 해당하는 필터링 캐싱부터 확인 */

	if (bIsFilterCacheDirty == false)
	{
		const auto* TailNode = mFilterCaches.GetTail();
		for (auto* CurNode = mFilterCaches.GetHead(); CurNode != TailNode; CurNode = CurNode->GetNextNode())
		{
			// 캐싱 히트!
			if (CurNode->GetValue().mFilterFlag == mCurFilterFlag)
			{
				mFilterCaches.RemoveNode(CurNode, false);
				mFilterCaches.AddHead(CurNode);
				UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurInventorySlotVMs);
				return;
			}
		}
	}
	else
	{
		bIsFilterCacheDirty = false;
		mFilterCaches.Empty();
	}

	/* 필터링 처리 후 결과 캐싱 */

	FInventoryFilter NewFilterCache;
	NewFilterCache.mFilterFlag = mCurFilterFlag;
	for (auto& SlotVM : mAllInventorySlotVMs)
	{
		NewFilterCache.TryAddSlotVM(SlotVM);
	}
	mFilterCaches.AddHead(NewFilterCache);

	if (mFilterCaches.Num() > mMaxFilterCacheCount)
	{
		mFilterCaches.RemoveNode(mFilterCaches.GetTail());
	}
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurInventorySlotVMs);
}

void UInventoryViewModel::ConnectToInventory(UInventoryComponent* const InventoryComponent)
{
	if (IsValid(InventoryComponent) == false)
	{
		return;
	}

	mInventoryComp = InventoryComponent;
	mAllInventorySlotVMs.Empty();

	/* 인벤토리 ViewModel 등록 */
	
	auto& InventoryItems = InventoryComponent->GetInventoryItems();
	for (int32 i = 0; i < InventoryItems.Num(); ++i)
	{
		auto* NewVM = NewObject<UInventorySlotViewModel>(this);
		mAllInventorySlotVMs.Add(NewVM);

		NewVM->SetEnabled(true);
		NewVM->SetSlotType(EInventorySlotType::InventorySlot);
		NewVM->SetSlotIndex(i);
		NewVM->SetItemInfo(InventoryItems[i]);
		NewVM->SetToolTipWidget(InventoryComponent->GetToolTipWidget());
		NewVM->OnTryToSwapInventorySlot.BindUObject(this, &UInventoryViewModel::SwapItemSlot);
	}

	/* 장비창 ViewModel 등록 */

	auto& EquipItems = InventoryComponent->GetEquipItems();
	for (int32 i = 0; i < EquipItems.Num(); ++i)
	{
		auto* NewVM = NewObject<UInventorySlotViewModel>(this);
		mAllEquipSlotVMs.Add(NewVM);

		NewVM->SetEnabled(true);
		NewVM->SetSlotType(EInventorySlotType::EquipSlot);
		NewVM->SetSlotIndex(i);
		NewVM->SetItemInfo(EquipItems[i]);
		NewVM->SetToolTipWidget(InventoryComponent->GetToolTipWidget());
		NewVM->OnTryToSwapInventorySlot.BindUObject(this, &UInventoryViewModel::SwapItemSlot);
	}

	/* 쓰레기통 ViewModel 등록 */

	{
		auto* NewVM = NewObject<UInventoryTrashCanViewModel>(this);
		mTrashVM = NewVM;
		NewVM->OnTryToClearInventorySlot.BindUObject(this, &UInventoryViewModel::ClearItemSlot);
	}

	InventoryComponent->OnUpdateInventoryItem.AddUObject(this, &UInventoryViewModel::UpdateInventoryItemSlot);
	InventoryComponent->OnChangeEquipItem.AddUObject(this, &UInventoryViewModel::UpdateEquipItemSlot);

	auto* PlayerChar = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	if (IsValid(PlayerChar) == false)
	{
		return;
	}
	PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetMaxHPAttribute()).AddLambda([this](const FOnAttributeChangeData& ChangeData) {
		UpdateHP();
		});
	PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetStaminaMaxAttribute()).AddLambda([this](const FOnAttributeChangeData& ChangeData) {
		UpdateStemina();
		});
	PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetAttackPowerAttribute()).AddLambda([this](const FOnAttributeChangeData& ChangeData) {
		UpdateAttackDamage();
		});
	PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetDefenseAttribute()).AddLambda([this](const FOnAttributeChangeData& ChangeData) {
		UpdateDefense();
		});
	PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetMoveSpeedAttribute()).AddLambda([this](const FOnAttributeChangeData& ChangeData) {
		UpdateMoveSpeed();
		});
	UpdateHP();
	UpdateStemina();
	UpdateAttackDamage();
	UpdateDefense();
	UpdateMoveSpeed();
}

void UInventoryViewModel::ShowView()
{
	SetVisiblity(true);
	OnChangeViewVisibility.Broadcast();
}

void UInventoryViewModel::CloseView()
{
	SetVisiblity(false);
	OnChangeViewVisibility.Broadcast();
}

void UInventoryViewModel::UpdateInventoryItemSlot(int32 SlotIndex, TSharedPtr<const FGameItemInfo> ItemInfo, TSharedPtr<const FGameItemInfo> PreItemInfo)
{
	UInventorySlotViewModel* UpdatedSlotVM = mAllInventorySlotVMs[SlotIndex];
	UpdatedSlotVM->SetItemInfo(ItemInfo);
	if (mFilterCaches.GetHead() != nullptr)
	{
		bool bWasChangedSlotCount;
		mFilterCaches.GetHead()->GetValue().UpdateSlotVM(UpdatedSlotVM, bWasChangedSlotCount);
		if (bWasChangedSlotCount == true)
		{
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurInventorySlotVMs);
		}
	}
	bIsFilterCacheDirty = true;
}

void UInventoryViewModel::UpdateEquipItemSlot(int32 SlotIndex, TSharedPtr<const FGameItemInfo> ItemInfo, TSharedPtr<const FGameItemInfo> PreItemInfo)
{
	UInventorySlotViewModel* UpdatedSlotVM = mAllEquipSlotVMs[SlotIndex];
	UpdatedSlotVM->SetItemInfo(ItemInfo);
}

void UInventoryViewModel::UpdateHP()
{
	auto* PlayerChar = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	if (IsValid(PlayerChar) == false)
	{
		return;
	}
	bool bFound;
	SetMaxHP(PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValue(UBaseAttributeSet::GetMaxHPAttribute(), bFound));
}

void UInventoryViewModel::UpdateStemina()
{
	auto* PlayerChar = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	if (IsValid(PlayerChar) == false)
	{
		return;
	}
	bool bFound;
	SetMaxStemina(PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValue(UPlayerAttributeSet::GetStaminaMaxAttribute(), bFound));
}

void UInventoryViewModel::UpdateMoveSpeed()
{
	auto* PlayerChar = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	if (IsValid(PlayerChar) == false)
	{
		return;
	}
	bool bFound;
	SetMoveSpeed(PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValue(UBaseAttributeSet::GetMoveSpeedAttribute(), bFound));
}

void UInventoryViewModel::UpdateAttackDamage()
{
	auto* PlayerChar = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	if (IsValid(PlayerChar) == false)
	{
		return;
	}
	bool bFound;
	SetAttackDamage(PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValue(UBaseAttributeSet::GetAttackPowerAttribute(), bFound));
}

void UInventoryViewModel::UpdateDefense()
{
	auto* PlayerChar = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	if (IsValid(PlayerChar) == false)
	{
		return;
	}
	bool bFound;
	SetDefense(PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValue(UBaseAttributeSet::GetDefenseAttribute(), bFound));
}

/************************
	  UPauseViewModel
*************************/

void UPauseViewModel::SetVisiblity(bool bWillBeVisiblity)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(bIsVisiblity, bWillBeVisiblity))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetUMGSequencePlayMode);
		auto* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();
		if (IsValid(MainPlayerController) == true)
		{
			if (bIsVisiblity == true)
			{
				MainPlayerController->IncreaseUIOnlyModeCount();
				MainPlayerController->SetPause(true);

				auto* SaveGameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
				if (IsValid(SaveGameSubsystem) == false)
				{
					return;
				}
				TObjectPtr<USavePlayer> CurSavePlayer = SaveGameSubsystem->GetCurSavePlayer();
				if (IsValid(CurSavePlayer) == false)
				{
					return;
				}
				SetStageName(FText::FromString(CurSavePlayer->GetStageName()));
				SetLevel(CurSavePlayer->GetLevel());
				SetSaveDate(CurSavePlayer->GetDate());
			}
			else
			{
				MainPlayerController->SetPause(false);
				MainPlayerController->DecreaseUIOnlyModeCount();
			}
		}
	}
}

bool UPauseViewModel::IsVisiblity() const
{
	return bIsVisiblity;
}

void UPauseViewModel::SetStageName(const FText& StageName)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mStageName, StageName))
	{
	}
}

const FText& UPauseViewModel::GetStageName() const
{
	return mStageName;
}

void UPauseViewModel::SetLevel(int32 Level)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mLevel, Level))
	{
	}
}

int32 UPauseViewModel::GetLevel() const
{
	return mLevel;
}

void UPauseViewModel::SetSaveDate(FDateTime SaveDate)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mSaveDate, SaveDate))
	{
	}
}

FDateTime UPauseViewModel::GetSaveDate() const
{
	return mSaveDate;
}

TEnumAsByte<EUMGSequencePlayMode::Type> UPauseViewModel::GetUMGSequencePlayMode() const
{
	return bIsVisiblity == true ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse;
}

void UPauseViewModel::ContinueGame()
{
	ToggleView();
}

void UPauseViewModel::ConfigSettings()
{
}

void UPauseViewModel::MoveToTitleMap()
{
	auto* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();
	if (IsValid(MainPlayerController) == true)
	{
		MainPlayerController->DecreaseUIOnlyModeCount();
	}

	auto* SaveGameSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(SaveGameSubSystem) == false)
	{
		return;
	}
	SaveGameSubSystem->SaveAllPlayers(true);
	SaveGameSubSystem->SetCurSavePlayer(nullptr);

	auto* WorldSettings = Cast<AACWorldSettings>(GetWorld()->GetWorldSettings());
	if (IsValid(WorldSettings) == false)
	{
		return;
	}
	auto* StageTableRow = WorldSettings->GetDataTableRow<FStageData>(EDataTableType::Map, TEXT("로딩"));
	if (StageTableRow != nullptr)
	{
		UGameplayStatics::OpenLevel(GetWorld(), *StageTableRow->mLevel.GetAssetName());
	}
}

void UPauseViewModel::ToggleView()
{
	SetVisiblity(!IsVisiblity());
	OnChangeViewVisibility.Broadcast();
}

/************************
  UMonsterHPBarViewModel
*************************/

void UMonsterHPBarViewModel::SetHpPercent(float HpPercent)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mHpPercent, HpPercent))
	{
	}
}

float UMonsterHPBarViewModel::GetHpPercent() const
{
	return mHpPercent;
}

void UMonsterHPBarViewModel::SetDisplayName(const FText& DisplayName)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mDisplayName, DisplayName))
	{
	}
}

const FText& UMonsterHPBarViewModel::GetDisplayName() const
{
	return mDisplayName;
}

void UMonsterHPBarViewModel::Initialize_Implementation()
{
	auto* BaseChar = Cast<ABaseCharacter>(GetOuter());
	if (IsValid(BaseChar) == false)
	{
		return;
	}
	SetDisplayName(BaseChar->GetDisplayName());
	BaseChar->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetHPAttribute()).AddLambda([this](const FOnAttributeChangeData& ChangeData) {
		UpdateHP();
		});
	BaseChar->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetMaxHPAttribute()).AddLambda([this](const FOnAttributeChangeData& ChangeData) {
		UpdateHP();
		});



	UpdateHP();
}

void UMonsterHPBarViewModel::UpdateHP()
{
	auto* BaseChar = Cast<ABaseCharacter>(GetOuter());
	if (IsValid(BaseChar) == false)
	{
		return;
	}

	bool bFound;
	float NewPercent;
	NewPercent = BaseChar->GetAbilitySystemComponent()->GetGameplayAttributeValue(UBaseAttributeSet::GetHPAttribute(), bFound);
	NewPercent /= BaseChar->GetAbilitySystemComponent()->GetGameplayAttributeValue(UBaseAttributeSet::GetMaxHPAttribute(), bFound);

	SetHpPercent(NewPercent);
}

/************************
	  UHUDViewModel
*************************/

void UHUDViewModel::SetHpPercent(float HpPercent)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mHpPercent, HpPercent))
	{
	}
}

float UHUDViewModel::GetHpPercent() const
{
	return mHpPercent;
}

void UHUDViewModel::SetSteminaPercent(float SteminaPercent)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mSteminaPercent, SteminaPercent))
	{
	}
}

float UHUDViewModel::GetSteminaPercent() const
{
	return mSteminaPercent;
}

void UHUDViewModel::SetPotionCount(int32 PotionCount)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mPotionCount, PotionCount))
	{
	}
}

int32 UHUDViewModel::GetPotionCount() const
{
	return mPotionCount;
}

void UHUDViewModel::SetDisplayName(const FText& DisplayName)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mDisplayName, DisplayName))
	{
	}
}

const FText& UHUDViewModel::GetDisplayName() const
{
	return mDisplayName;
}

void UHUDViewModel::SetCanSkill(bool CanSkill)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(bCanSkill, CanSkill))
	{
	}
}

bool UHUDViewModel::GetCanSkill() const
{
	return bCanSkill;
}

void UHUDViewModel::SetCanRoll(bool CanRoll)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(bCanRoll, CanRoll))
	{
	}
}

bool UHUDViewModel::GetCanRoll() const
{
	return bCanRoll;
}

void UHUDViewModel::Initialize_Implementation()
{
	auto* PlayerChar = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	if (IsValid(PlayerChar) == false)
	{
		return;
	}
	SetDisplayName(PlayerChar->GetDisplayName());
	PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetHPAttribute()).AddLambda([this](const FOnAttributeChangeData& ChangeData) {
		UpdateHP();
		});
	PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetMaxHPAttribute()).AddLambda([this](const FOnAttributeChangeData& ChangeData) {
		UpdateHP();
		});
	PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetStaminaAttribute()).AddLambda([this](const FOnAttributeChangeData& ChangeData) {
		UpdateStemina();
		});
	PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetStaminaMaxAttribute()).AddLambda([this](const FOnAttributeChangeData& ChangeData) {
		UpdateStemina();
		});
	PlayerChar->GetInventoryComponent()->OnUpdateInventoryItem.AddUObject(this, &UHUDViewModel::UpdatePotionCount);
	PlayerChar->GetAbilitySystemComponent()->RegisterGameplayTagEvent(AC_PlayerState::Player_State_Rolling, EGameplayTagEventType::NewOrRemoved).AddLambda([this](FGameplayTag, int32 Count) {
		UpdateRoll();
		});
	PlayerChar->GetAbilitySystemComponent()->RegisterGameplayTagEvent(AC_PlayerState::Player_Skill_Active01, EGameplayTagEventType::NewOrRemoved).AddLambda([this](FGameplayTag, int32 Count) {
		UpdateSkill();
		});
	UpdateHP();
	UpdateStemina();
	UpdateRoll();
	UpdateSkill();
}

void UHUDViewModel::UpdateHP()
{
	auto* PlayerChar = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	if (IsValid(PlayerChar) == false)
	{
		return;
	}

	bool bFound;
	float NewPercent;
	NewPercent = PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValue(UBaseAttributeSet::GetHPAttribute(), bFound);
	NewPercent /= PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValue(UBaseAttributeSet::GetMaxHPAttribute(), bFound);

	SetHpPercent(NewPercent);
}

void UHUDViewModel::UpdateStemina()
{
	auto* PlayerChar = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	if (IsValid(PlayerChar) == false)
	{
		return;
	}

	bool bFound;
	float NewPercent;
	NewPercent = PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValue(UPlayerAttributeSet::GetStaminaAttribute(), bFound);
	NewPercent /= PlayerChar->GetAbilitySystemComponent()->GetGameplayAttributeValue(UPlayerAttributeSet::GetStaminaMaxAttribute(), bFound);

	SetSteminaPercent(NewPercent);
}

void UHUDViewModel::UpdatePotionCount(int32 InventoryIndex, TSharedPtr<const FGameItemInfo> ItemInfo, TSharedPtr<const FGameItemInfo> PreItemInfo)
{
	int32 Offset = 0;
	if (ItemInfo != nullptr && ItemInfo->mType == EItemType::Potion)
	{
		++Offset;
	}
	if (PreItemInfo != nullptr && PreItemInfo->mType == EItemType::Potion)
	{
		--Offset;
	}
	SetPotionCount(GetPotionCount() + Offset);
}

void UHUDViewModel::UpdateSkill()
{
	auto* PlayerChar = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	if (IsValid(PlayerChar) == false)
	{
		return;
	}

	SetCanSkill(PlayerChar->GetAbilitySystemComponent()->HasMatchingGameplayTag(AC_PlayerState::Player_Skill_Active01) == false);
}

void UHUDViewModel::UpdateRoll()
{
	auto* PlayerChar = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	if (IsValid(PlayerChar) == false)
	{
		return;
	}

	SetCanRoll(PlayerChar->GetAbilitySystemComponent()->HasMatchingGameplayTag(AC_PlayerState::Player_State_Rolling) == false);
}

/************************
	UGameOverViewModel
*************************/

void UGameOverViewModel::Initialize_Implementation()
{
	Super::Initialize_Implementation();

	auto* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();
	if (IsValid(MainPlayerController) == true)
	{
		MainPlayerController->IncreaseUIOnlyModeCount();
	}
}

void UGameOverViewModel::MoveToTitleMap()
{
	auto* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();
	if (IsValid(MainPlayerController) == true)
	{
		MainPlayerController->DecreaseUIOnlyModeCount();
	}
	auto* SaveGameSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(SaveGameSubSystem) == false)
	{
		return;
	}
	SaveGameSubSystem->SetCurSavePlayer(nullptr);

	auto* WorldSettings = Cast<AACWorldSettings>(GetWorld()->GetWorldSettings());
	if (IsValid(WorldSettings) == false)
	{
		return;
	}
	auto* StageTableRow = WorldSettings->GetDataTableRow<FStageData>(EDataTableType::Map, TEXT("로딩"));
	if (StageTableRow != nullptr)
	{
		UGameplayStatics::OpenLevel(GetWorld(), *StageTableRow->mLevel.GetAssetName());
	}
}