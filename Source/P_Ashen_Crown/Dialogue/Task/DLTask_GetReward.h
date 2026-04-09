#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/TableRow/DialogueTableRow.h"
#include "P_Ashen_Crown/Component/InventoryComponent.h"
#include "DLTask_GetReward.generated.h"

USTRUCT(BlueprintType)
struct FRewardItemInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "RewardItemInfo", meta = (DisplayName = "Count"))
	int32 mCount;

	UPROPERTY(EditDefaultsOnly, Category = "RewardItemInfo", meta = (DisplayName = "ItemType"))
	EItemType mItemType;

	UPROPERTY(EditDefaultsOnly, Category = "RewardItemInfo", meta = (DisplayName = "ItemKey"))
	FName mItemKey;
};

UCLASS()
class P_ASHEN_CROWN_API UDLTaskParam_GetReward : public UDialogueResultParam
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "GetReward", meta = (DisplayName = "RewardItems"))
	TArray<FRewardItemInfo> mRewardItems;
};

/**
 * 보상을 받는 Dialogue Task
 */
UCLASS()
class P_ASHEN_CROWN_API UDLTask_GetReward : public UDialogueResultTask
{
	GENERATED_BODY()
	
public:
	virtual EDialogueResult ExecuteTask_Implementation(UDialogueResultParam* Param, AActor* Questioner, AActor* Respondent) override;
};
