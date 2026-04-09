#include "SaveGameSubsystem.h"
#include "P_Ashen_Crown/SaveGame/SavePlayer.h"
#include "P_Ashen_Crown/Setting/Runtime/UserGameSettingTracker.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedActionKeyMapping.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#include "GameFramework/GameUserSettings.h"
#include "P_Ashen_Crown/Singleton/Subsystem/GameEventSubsystem.h"
#include "P_Ashen_Crown/Setting/ACWorldSettings.h"
#include "P_Ashen_Crown/Component/InventoryComponent.h"

#include "P_Ashen_Crown/GAS/GASCore.h"
#include "P_Ashen_Crown/GAS/BaseAttributeSet.h"

USaveGameSubsystem::USaveGameSubsystem() : mMaxSavePlayerCount(1), mSavePlayers(), mCurSavePlayer(nullptr), bWasLoadedSavePlayer(false), bWasLoadedUserOptions(false)
{
	mSettingTracker = CreateDefaultSubobject<UUserGameSettingTracker>(TEXT("SettingTracker"));
}

void USaveGameSubsystem::LoadAllPlayers()
{
	if (bWasLoadedSavePlayer == true)
	{
		return;
	}
	bWasLoadedSavePlayer = true;

	mSavePlayers.Empty();
	mSavePlayerOrigins.Empty();
	for (int i = 0; i < mMaxSavePlayerCount; ++i)
	{
		auto* SavePlayerData = Cast<USavePlayer>(UGameplayStatics::LoadGameFromSlot(mSavePlayerFileName + FString::FromInt(i), 0));
		if (SavePlayerData == nullptr)
		{
			break;
		}
		//SavePlayerData->Rename(nullptr, this);
		mSavePlayers.AddTail(SavePlayerData);
		mSavePlayerOrigins.Add(SavePlayerData);
	}

	auto* EventSubsystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
	EventSubsystem->RegisterLoadEventSolver(FOnLoadEventData::CreateUObject(this, &USaveGameSubsystem::LoadEventData));

	OnChangePlayerSaveList.Broadcast(mSavePlayers);
}

void USaveGameSubsystem::SaveAllPlayers(bool bAutoUpdateCurPlayer)
{
	AddCurPlayer(bAutoUpdateCurPlayer);
	auto* CurNode = mSavePlayers.GetHead();
	int i = 0;
	for (; i < mSavePlayers.Num(); ++i)
	{
		if (UGameplayStatics::SaveGameToSlot(CurNode->GetValue(), mSavePlayerFileName + FString::FromInt(i), 0) == false)
		{
			break;
		}
		CurNode = CurNode->GetNextNode();
	}
	for (; i < mMaxSavePlayerCount; ++i)
	{
		if (UGameplayStatics::DeleteGameInSlot(mSavePlayerFileName + FString::FromInt(i), 0) == false)
		{
			break;
		}
	}
}

int32 USaveGameSubsystem::GetMaxSavePlayerCount() const
{
	return mMaxSavePlayerCount;
}

TObjectPtr<USavePlayer> USaveGameSubsystem::GetSavePlayer(int32 Index)
{
	if (mSavePlayers.Num() >= Index || Index < 0)
	{
		return nullptr;
	}
	auto CurNode = mSavePlayers.GetHead();
	for (int i = 1; i < Index; ++i)
	{
		CurNode = CurNode->GetNextNode();
	}
	return CurNode->GetValue();
}

const TDoubleLinkedList<TObjectPtr<USavePlayer>>& USaveGameSubsystem::GetSavePlayers()
{
	return mSavePlayers;
}

TObjectPtr<USavePlayer> USaveGameSubsystem::GetCurSavePlayer()
{
	return mCurSavePlayer;
}

void USaveGameSubsystem::SetCurSavePlayer(TObjectPtr<USavePlayer> CurSavePlayer)
{
	mCurSavePlayer = CurSavePlayer;

	if (mCurSavePlayer != nullptr)
	{
		auto* EventSubsystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
		EventSubsystem->LoadEvents();
	}
}

void USaveGameSubsystem::AddSavePlayer(TObjectPtr<USavePlayer> SavePlayer)
{
	if (IsValid(SavePlayer) == true)
	{
		if (mSavePlayers.Num() == mMaxSavePlayerCount)
		{
			mSavePlayers.RemoveNode(mSavePlayers.GetTail());
		}
		mSavePlayers.AddHead(SavePlayer);
		mSavePlayerOrigins.Add(SavePlayer);
		OnChangePlayerSaveList.Broadcast(mSavePlayers);
	}
}

void USaveGameSubsystem::AddCurPlayer(bool bAutoUpdateCurPlayer)
{
	if (IsValid(mCurSavePlayer) == true)
	{
		USavePlayer* SaveCurPlayer = mCurSavePlayer->Clone();

		if (bAutoUpdateCurPlayer == true)
		{
			// 시간 저장
			SaveCurPlayer->SetDate(FDateTime::Now());

			/* 현재 레벨 저장 */

			auto* ACWorldSettings = Cast<AACWorldSettings>(GetGameInstance()->GetWorld()->GetWorldSettings());
			if (IsValid(ACWorldSettings) == true)
			{
				SaveCurPlayer->SetStageName(ACWorldSettings->GetStageName());
			}

			/* 캐릭터 트랜스폼 저장 */

			APawn* PlayerPawn = GetGameInstance()->GetFirstLocalPlayerController()->GetPawn();
			if (IsValid(PlayerPawn) == true)
			{
				SaveCurPlayer->SetPosition(PlayerPawn->GetActorLocation());
				SaveCurPlayer->SetRotation(PlayerPawn->GetActorRotation());
			}

			/* 캐릭터 HP 저장 */

			auto* ASCInterface = Cast<IAbilitySystemInterface>(GetGameInstance()->GetFirstLocalPlayerController()->GetPawn());
			if (ASCInterface != nullptr)
			{
				bool bIsFound;
				SaveCurPlayer->SetHP(ASCInterface->GetAbilitySystemComponent()->GetGameplayAttributeValue(UBaseAttributeSet::GetHPAttribute(), bIsFound));
			}

			/* 인벤토리 저장 */

			if (IsValid(PlayerPawn) == true)
			{
				auto* InventoryComp = PlayerPawn->FindComponentByClass<UInventoryComponent>();
				if (IsValid(InventoryComp) == true)
				{
					TArray<FSaveItemKeyInfo> EquipItemKeys;
					for (auto& Item : InventoryComp->GetEquipItems())
					{
						EquipItemKeys.Add(Item == nullptr ? FSaveItemKeyInfo{TEXT(""), EItemType::None} : FSaveItemKeyInfo{Item->mKeyName, Item->mType});
					}
					SaveCurPlayer->SetEquipItemKeys(EquipItemKeys);

					TArray<FSaveItemKeyInfo> InvenItemKeys;
					for (auto& Item : InventoryComp->GetInventoryItems())
					{
						InvenItemKeys.Add(Item == nullptr ? FSaveItemKeyInfo{ TEXT(""), EItemType::None } : FSaveItemKeyInfo{ Item->mKeyName, Item->mType });
					}
					SaveCurPlayer->SetInventoryItemKeys(InvenItemKeys);
				}
			}

			/* 이벤트 트리거 저장 */

			auto* EventSubsystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
			for (auto& EventContext : EventSubsystem->GetAllEventContext())
			{
				if (EventContext.Value.bIsTransient == false)
				{
					SaveCurPlayer->SetTiggeredEvent(EventContext.Key, EventContext.Value.bIsTriggered);
				}
			}
		}
		
		AddSavePlayer(SaveCurPlayer);
	}
}

void USaveGameSubsystem::RemoveSavePlayer(TObjectPtr<USavePlayer> TargetSavePlayer)
{
	if (IsValid(TargetSavePlayer) == true)
	{
		auto* Node = mSavePlayers.FindNode(TargetSavePlayer);
		mSavePlayers.RemoveNode(Node);
		mSavePlayerOrigins.Remove(Node->GetValue());
		OnChangePlayerSaveList.Broadcast(mSavePlayers);
	}
}

void USaveGameSubsystem::RemoveSavePlayer(int32 TargetIndex)
{
	if (mSavePlayers.Num() >= TargetIndex || TargetIndex < 0)
	{
		return;
	}
	auto CurNode = mSavePlayers.GetHead();
	for (int i = 1; i < TargetIndex; ++i)
	{
		CurNode = CurNode->GetNextNode();
	}
	mSavePlayers.RemoveNode(CurNode);
	mSavePlayerOrigins.Remove(CurNode->GetValue());
	OnChangePlayerSaveList.Broadcast(mSavePlayers);
}

void USaveGameSubsystem::LoadEventData(const FName& Key, bool& bWasTriggered)
{
	if (mCurSavePlayer == nullptr)
	{
		return;
	}

	if (mCurSavePlayer->WasTriggeredEvent(Key) == true)
	{
		bWasTriggered = true;
	}
	else
	{
		bWasTriggered = false;
	}
}

void USaveGameSubsystem::LoadOptionSetting()
{
	if (bWasLoadedUserOptions == true)
	{
		return;
	}
	bWasLoadedUserOptions = true;

	if (IsValid(mSettingTracker) == false)
	{
		return;
	}
	mSettingTracker->RegisterLinker<UUserEnhancedInputSettingLinker>();
	mSettingTracker->RegisterLinker<UUserGraphicsSettingLinker>();
	mSettingTracker->RegisterLinker<UUserSoundSettingLinker>();
}

void USaveGameSubsystem::SaveOptionSetting()
{
	if (IsValid(mSettingTracker) == false)
	{
		return;
	}
	mSettingTracker->SaveSettings();
}

TObjectPtr<UUserGameSettingTracker> USaveGameSubsystem::GetUserGameSettingTracker()
{
	return mSettingTracker;
}

