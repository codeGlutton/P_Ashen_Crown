#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P_Ashen_Crown/Component/InventoryComponent.h"
#include "ACGameLibrary.generated.h"

/**
 * Ashen Crwon 게임 헬퍼 함수들
 */
UCLASS()
class P_ASHEN_CROWN_API UACGameLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static TSharedPtr<FGameItemInfo> CreateGameItem(UWorld* World, EItemType ItemType, const FName& KeyName);
	static TSharedPtr<FGameItemInfo> CreateGameEquipItem(UWorld* World, const FName& KeyName);
	static TSharedPtr<FGameItemInfo> CreateGamePotionItem(UWorld* World, const FName& KeyName);
	static TSharedPtr<FGameItemInfo> CreateGameRuneItem(UWorld* World, const FName& KeyName);
};
