#include "ACGameLibrary.h"
#include "P_Ashen_Crown/Setting/ACWorldSettings.h"

TSharedPtr<FGameItemInfo> UACGameLibrary::CreateGameItem(UWorld* World, EItemType ItemType, const FName& KeyName)
{
	switch (ItemType)
	{
	case EItemType::Equip:
		return CreateGameEquipItem(World, KeyName);
	case EItemType::Potion:
		return CreateGamePotionItem(World, KeyName);
	case EItemType::Rune:
		return CreateGameRuneItem(World, KeyName);
	}
	return nullptr;
}

TSharedPtr<FGameItemInfo> UACGameLibrary::CreateGameEquipItem(UWorld* World, const FName& KeyName)
{
	if (IsValid(World) == false)
	{
		return nullptr;
	}
	auto* WorldSettings = Cast<AACWorldSettings>(World->GetWorldSettings());
	if (IsValid(WorldSettings) == false)
	{
		return nullptr;
	}

	auto* GameEquipInfo = WorldSettings->GetDataTableRow<FGameEquipInfo>(EDataTableType::Equip, KeyName);
	if (GameEquipInfo == nullptr)
	{
		return nullptr;
	}
	auto NewEquipInfo = MakeShared<FGameEquipInfo>();
	*NewEquipInfo = *GameEquipInfo;
	return NewEquipInfo;
}

TSharedPtr<FGameItemInfo> UACGameLibrary::CreateGamePotionItem(UWorld* World, const FName& KeyName)
{
	if (IsValid(World) == false)
	{
		return nullptr;
	}
	auto* WorldSettings = Cast<AACWorldSettings>(World->GetWorldSettings());
	if (IsValid(WorldSettings) == false)
	{
		return nullptr;
	}

	auto* GamePotionInfo = WorldSettings->GetDataTableRow<FGamePotionInfo>(EDataTableType::Potion, KeyName);
	if (GamePotionInfo == nullptr)
	{
		return nullptr;
	}
	auto NewPotionInfo = MakeShared<FGamePotionInfo>();
	*NewPotionInfo = *GamePotionInfo;
	return NewPotionInfo;
}

TSharedPtr<FGameItemInfo> UACGameLibrary::CreateGameRuneItem(UWorld* World, const FName& KeyName)
{
	if (IsValid(World) == false)
	{
		return nullptr;
	}
	auto* WorldSettings = Cast<AACWorldSettings>(World->GetWorldSettings());
	if (IsValid(WorldSettings) == false)
	{
		return nullptr;
	}

	auto* GameRuneInfo = WorldSettings->GetDataTableRow<FGameRuneInfo>(EDataTableType::Rune, KeyName);
	if (GameRuneInfo == nullptr)
	{
		return nullptr;
	}
	auto NewRuneInfo = MakeShared<FGameRuneInfo>();
	*NewRuneInfo = *GameRuneInfo;
	return NewRuneInfo;
}

