#include "DLTask_GetReward.h"
#include "P_Ashen_Crown/ACGameLibrary.h"

EDialogueResult UDLTask_GetReward::ExecuteTask_Implementation(UDialogueResultParam* Param, AActor* Questioner, AActor* Respondent)
{
	if (Questioner == nullptr || Respondent == nullptr)
	{
		return EDialogueResult::End;
	}

	auto* TaskParam = Cast<UDLTaskParam_GetReward>(Param);
	if (IsValid(TaskParam) == false)
	{
		return EDialogueResult::End;
	}

	auto* InventoryComp = Respondent->FindComponentByClass<UInventoryComponent>();
	if (IsValid(InventoryComp) == false)
	{
		return EDialogueResult::End;
	}

	for (auto& RewardItemData : TaskParam->mRewardItems)
	{
		for (int32 i = 0; i < RewardItemData.mCount; ++i)
		{
			TSharedPtr<FGameItemInfo> ItemObj = UACGameLibrary::CreateGameItem(Respondent->GetWorld(), RewardItemData.mItemType, RewardItemData.mItemKey);
			if (InventoryComp->AddItem(ItemObj) == false)
			{
				return EDialogueResult::End;
			}
		}
	}

	return EDialogueResult::End;
}
