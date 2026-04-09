#include "InventoryComponent.h"
#include "Blueprint/UserWidget.h"

bool FGameItemInfo::CanEquip() const
{
	return mType == EItemType::None || mType == EItemType::Equip;
}

const FText& FGameEquipInfo::GetAbilityText() const
{
	mAbilityText = FText::FromString(
		FString::Printf(TEXT("공격력 : %.2f\n방어력 : %.2f\n이동속도 : %.2f\n스테미나 : %.2f\nHP : %.2f"),
			mAttackPower,
			mDefense,
			mMoveSpeed,
			mStamina,
			mHP
		)
	);
	return mAbilityText;
}

const FText& FGamePotionInfo::GetAbilityText() const
{
	mAbilityText = FText::FromString(
		FString::Printf(TEXT("회복량 : %.2f"),
			mHealAmount
		)
	);
	return mAbilityText;
}

const FText& FGameRuneInfo::GetAbilityText() const
{
	mAbilityText = FText::FromString(
		FString::Printf(TEXT("공격력 증가 퍼센트 : %.2f\n방어력 증가 퍼센트 : %.2f\n이동속도 증가 퍼센트 : %.2f\n스테미나 증가 퍼센트 : %.2f\nHP 증가 퍼센트 : %.2f"),
			mAttackPowerPercent,
			mDefensePercent,
			mMoveSpeedPercent,
			mStaminaPercent,
			mHPPercent
		)
	);
	return mAbilityText;
}


/************************
   UInventoryComponent
*************************/

UInventoryComponent::UInventoryComponent() :
	mCurItemCount(0)
{
	PrimaryComponentTick.bCanEverTick = false;
	mInventoryItems.Init(nullptr, mMaxItemCount);
	mEquipItems.Init(nullptr, StaticCast<int32>(EEquipType::Count));
}

void UInventoryComponent::RegisterInventoryView(IInventoryViewHandler* Handler, UUserWidget* ToolTipWidget)
{
	if (Handler != nullptr)
	{
		mInventoryHandler.SetObject(Handler->_getUObject());
		mInventoryHandler.SetInterface(Handler);
		mToolTipWidget = ToolTipWidget;
		Handler->ConnectToInventory(this);
	}
}

void UInventoryComponent::UnregisterInventoryView()
{
	CloseInventory();
	mInventoryHandler = nullptr;
}

void UInventoryComponent::LoadEquipItem(int32 EquipIndex, TSharedPtr<FGameItemInfo> Item)
{
	TSharedPtr<FGameItemInfo> PreEquipItem = mEquipItems[EquipIndex];
	mEquipItems[EquipIndex] = Item;
	OnChangeEquipItem.Broadcast(EquipIndex, mEquipItems[EquipIndex], PreEquipItem);
}

void UInventoryComponent::LoadInventoryItem(int32 InventoryIndex, TSharedPtr<FGameItemInfo> Item)
{
	mInventoryItems[InventoryIndex] = Item;
	if (Item != nullptr)
	{
		++mCurItemCount;
	}
	OnUpdateInventoryItem.Broadcast(InventoryIndex, mInventoryItems[InventoryIndex], nullptr);
}

void UInventoryComponent::OpenInventory()
{
	if (mInventoryHandler != nullptr)
	{
		mInventoryHandler->ShowView();
	}
	bIsOpen = true;
}

void UInventoryComponent::CloseInventory()
{
	bIsOpen = false;
	if (mInventoryHandler != nullptr)
	{
		mInventoryHandler->CloseView();
	}
}

void UInventoryComponent::ToggleInventory()
{
	if (bIsOpen == true)
	{
		CloseInventory();
	}
	else
	{
		OpenInventory();
	}
}

const TArray<TSharedPtr<FGameItemInfo>>& UInventoryComponent::GetInventoryItems() const
{
	return mInventoryItems;
}

const TArray<TSharedPtr<FGameItemInfo>>& UInventoryComponent::GetEquipItems() const
{
	return mEquipItems;
}

UUserWidget* UInventoryComponent::GetToolTipWidget() const
{
	return mToolTipWidget;
}

bool UInventoryComponent::IsInventoryFull() const
{
	return mMaxItemCount <= mCurItemCount;
}

bool UInventoryComponent::AddItem(TSharedPtr<FGameItemInfo> NewItem)
{
	if (NewItem == nullptr || IsInventoryFull() == true)
	{
		return false;
	}
	for (int32 i = 0; i < mMaxItemCount; ++i)
	{
		if (mInventoryItems[i] == nullptr)
		{
			mInventoryItems[i] = NewItem;
			++mCurItemCount;
			OnUpdateInventoryItem.Broadcast(i, mInventoryItems[i], nullptr);
			return true;
		}
	}
	return false;
}

void UInventoryComponent::AddTestEquipItem(UDataTable* Table, const FName& Name)
{
	if (IsValid(Table) == false)
	{
		return;
	}
	auto* TestRow = Table->FindRow<FGameEquipInfo>(Name, TEXT(""));
	AddItem(*TestRow);
}

bool UInventoryComponent::RemoveInventoryItem(int32 InvenotryIndex)
{
	if (mInventoryItems.IsValidIndex(InvenotryIndex) == false || mInventoryItems[InvenotryIndex] == nullptr)
	{
		return false;
	}

	TSharedPtr<FGameItemInfo> PreItem = mInventoryItems[InvenotryIndex];
	mInventoryItems[InvenotryIndex] = nullptr;
	--mCurItemCount;
	OnUpdateInventoryItem.Broadcast(InvenotryIndex, mInventoryItems[InvenotryIndex], PreItem);

	return true;
}

bool UInventoryComponent::RemoveEquipItem(int32 EquipIndex)
{
	if (mEquipItems.IsValidIndex(EquipIndex) == false || mEquipItems[EquipIndex] == nullptr)
	{
		return false;
	}

	auto PreEquipItem = mEquipItems[EquipIndex];
	mEquipItems[EquipIndex] = nullptr;
	--mCurItemCount;
	OnChangeEquipItem.Broadcast(EquipIndex, mEquipItems[EquipIndex], PreEquipItem);

	return true;
}

bool UInventoryComponent::RemoveItem(TSharedPtr<const FGameItemInfo>& TargetItem)
{
	for (int32 i = 0; i < mInventoryItems.Num(); ++i)
	{
		if (mInventoryItems[i] == TargetItem)
		{
			return RemoveInventoryItem(i);
		}
	}
	for (int32 i = 0; i < mEquipItems.Num(); ++i)
	{
		if (mEquipItems[i] == TargetItem)
		{
			return RemoveEquipItem(i);
		}
	}

	return false;
}

bool UInventoryComponent::SwapItems(int32 LhsInventoryIndex, int32 RhsInventoryIndex)
{
	if (mInventoryItems.IsValidIndex(LhsInventoryIndex) == false || mInventoryItems.IsValidIndex(RhsInventoryIndex) == false)
	{
		return false;
	}

	Swap(mInventoryItems[LhsInventoryIndex], mInventoryItems[RhsInventoryIndex]);
	OnUpdateInventoryItem.Broadcast(LhsInventoryIndex, mInventoryItems[LhsInventoryIndex], mInventoryItems[RhsInventoryIndex]);
	OnUpdateInventoryItem.Broadcast(RhsInventoryIndex, mInventoryItems[RhsInventoryIndex], mInventoryItems[LhsInventoryIndex]);

	return true;
}

bool UInventoryComponent::EquipItem(int32 EquipIndex, int32 InventoryIndex)
{
	if (mEquipItems.IsValidIndex(EquipIndex) == false || mInventoryItems.IsValidIndex(InventoryIndex) == false)
	{
		return false;
	}

	TSharedPtr<FGameItemInfo>& PreItem = mEquipItems[EquipIndex];
	TSharedPtr<FGameItemInfo>& NextItem = mInventoryItems[InventoryIndex];
	if (NextItem == nullptr)
	{
		if (PreItem == nullptr)
		{
			return false;
		}
		else
		{
			++mCurItemCount;
		}
	}
	else
	{
		if (PreItem == nullptr)
		{
			--mCurItemCount;
		}
		if (NextItem->CanEquip() == false)
		{
			return false;
		}
		TSharedPtr<FGameEquipInfo> NextEquipItem = StaticCastSharedPtr<FGameEquipInfo>(NextItem);
		if (NextEquipItem->mEquipType != StaticCast<EEquipType>(EquipIndex))
		{
			return false;
		}
	}

	Swap(PreItem, NextItem);
	OnUpdateInventoryItem.Broadcast(InventoryIndex, mInventoryItems[InventoryIndex], mEquipItems[EquipIndex]);
	OnChangeEquipItem.Broadcast(EquipIndex, mEquipItems[EquipIndex], mInventoryItems[InventoryIndex]);
	return true;
}
