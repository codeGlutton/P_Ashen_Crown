#include "TitleViewModel.h"

#include "Engine/AssetManager.h"
#include "P_Ashen_Crown/Setting/ACWorldSettings.h"
#include "P_Ashen_Crown/TableRow/StageData.h"

#include "P_Ashen_Crown/SaveGame/SavePlayer.h"
#include "P_Ashen_Crown/Singleton/Subsystem/SaveGameSubsystem.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedActionKeyMapping.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#include "GameFramework/GameUserSettings.h"

#include "P_Ashen_Crown/Setting/Runtime/UserGameSettingTracker.h"

/************************
  ULoadGameSlotViewModel
*************************/

ULoadGameSlotViewModel::ULoadGameSlotViewModel() : mSavePlayer(nullptr), mStageImage(nullptr), bIsSelected(false)
{
}

void ULoadGameSlotViewModel::SetSavePlayer(USavePlayer* SavePlayer)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mSavePlayer, SavePlayer))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetLevel);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDate);

		if (IsValid(mSavePlayer) == false)
		{
			return;
		}

		SetStageName(FText::FromString(mSavePlayer->GetStageName()));
		auto* WorldSettings = Cast<AACWorldSettings>(GetWorld()->GetWorldSettings());
		if (IsValid(WorldSettings) == false)
		{
			return;
		}

		FString StageKey = *mSavePlayer->GetStageName();
		StageKey.RemoveSpacesInline();
		auto* StageTableRow = WorldSettings->GetDataTableRow<FStageData>(EDataTableType::Map, *StageKey);
		if (StageTableRow != nullptr)
		{
			UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(StageTableRow->mImage.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this, LoadedAssetPath = StageTableRow->mImage]()
				{
					SetStageImage(LoadedAssetPath.Get());
				}));
		}
	}
}

USavePlayer* ULoadGameSlotViewModel::GetSavePlayer() const
{
	return mSavePlayer;
}

void ULoadGameSlotViewModel::SetStageName(const FText& StageName)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mStageName, StageName))
	{
	}
}

const FText& ULoadGameSlotViewModel::GetStageName() const
{
	return mStageName;
}

void ULoadGameSlotViewModel::SetStageImage(UTexture2D* StageImage)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mStageImage, StageImage))
	{
	}
}

UTexture2D* ULoadGameSlotViewModel::GetStageImage() const
{
	return mStageImage;
}

void ULoadGameSlotViewModel::Select(bool bIsWillSelected)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(bIsSelected, bIsWillSelected))
	{
	}
}

bool ULoadGameSlotViewModel::IsSelected() const
{
	return bIsSelected;
}

int32 ULoadGameSlotViewModel::GetLevel() const
{
	if (IsValid(mSavePlayer) == true)
	{
		return mSavePlayer->GetLevel();
	}
	return 0;
}

FDateTime ULoadGameSlotViewModel::GetDate() const
{
	if (IsValid(mSavePlayer) == true)
	{
		return mSavePlayer->GetDate();
	}
	return FDateTime::MinValue();
}

/************************
	ULoadGameViewModel
*************************/

ULoadGameViewModel::ULoadGameViewModel() : mLoadGameSlotVMs(), mSelectedSlotVM(nullptr)
{
}

void ULoadGameViewModel::SetSelectedSlotVM(ULoadGameSlotViewModel* SelectedSlotVM)
{
	if (IsValid(mSelectedSlotVM) == true && mSelectedSlotVM->IsSelected() == true)
	{
		mSelectedSlotVM->Select(false);
	}

	if (UE_MVVM_SET_PROPERTY_VALUE(mSelectedSlotVM, SelectedSlotVM))
	{
	}
}

ULoadGameSlotViewModel* ULoadGameViewModel::GetSelectedSlotVM() const
{
	return mSelectedSlotVM;
}

void ULoadGameViewModel::NotifyFromSelectedSlot(UObject* Obj, UE::FieldNotification::FFieldId FieIdId)
{
	auto* SlotVM = Cast<ULoadGameSlotViewModel>(Obj);
	if (IsValid(SlotVM) == true && SlotVM->IsSelected() == true)
	{
		SetSelectedSlotVM(SlotVM);
	}
}

void ULoadGameViewModel::Initialize_Implementation()
{
	Super::Initialize_Implementation();

	auto SaveGameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(SaveGameSubsystem) == false)
	{
		return;
	}
	UpdateLoadGameSlotVM(SaveGameSubsystem->GetSavePlayers());
	SaveGameSubsystem->OnChangePlayerSaveList.AddUObject(this, &ULoadGameViewModel::UpdateLoadGameSlotVM);
}

void ULoadGameViewModel::Deinitialize_Implementation()
{
	Super::Deinitialize_Implementation();

	const int VMSize = mLoadGameSlotVMs.Num();
	for (int32 i = 0; i < VMSize; ++i)
	{
		mLoadGameSlotVMs[i]->RemoveFieldValueChangedDelegate(ULoadGameSlotViewModel::FFieldNotificationClassDescriptor::bIsSelected, mSelectedSlotNotifyHandles[i]);
	}
	mSelectedSlotNotifyHandles.Empty();
	mLoadGameSlotVMs.Empty();
}

void ULoadGameViewModel::DeleteSelectedSlot()
{
	auto* SaveGameSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(mSelectedSlotVM) == false || IsValid(SaveGameSubSystem) == false)
	{
		return;
	}
	ULoadGameSlotViewModel* PreSelectedSlotVM = mSelectedSlotVM;
	SetSelectedSlotVM(nullptr);
	SaveGameSubSystem->RemoveSavePlayer(PreSelectedSlotVM->GetSavePlayer());
	SaveGameSubSystem->SaveAllPlayers(true);
}

void ULoadGameViewModel::PlayGameUsingSelectedSlot()
{
	auto* SaveGameSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(mSelectedSlotVM) == false || IsValid(SaveGameSubSystem) == false)
	{
		return;
	}

	SaveGameSubSystem->SetCurSavePlayer(mSelectedSlotVM->GetSavePlayer()->Clone());
}

void ULoadGameViewModel::UpdateLoadGameSlotVM(const TDoubleLinkedList<TObjectPtr<USavePlayer>>& PlayerSaveList)
{
	auto* SaveGameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(SaveGameSubsystem) == false)
	{
		return;
	}

	const int VMSize = mLoadGameSlotVMs.Num();

	for (int32 i = 0; i < VMSize; ++i)
	{
		mLoadGameSlotVMs[i]->RemoveFieldValueChangedDelegate(ULoadGameSlotViewModel::FFieldNotificationClassDescriptor::bIsSelected, mSelectedSlotNotifyHandles[i]);
	}
	mSelectedSlotNotifyHandles.Empty();
	mLoadGameSlotVMs.Empty();

	auto& SavePlayers = SaveGameSubsystem->GetSavePlayers();
	for (auto& SavePlayer : SavePlayers)
	{
		auto* NewVM = NewObject<ULoadGameSlotViewModel>(this);
		mLoadGameSlotVMs.Add(NewVM);
		mSelectedSlotNotifyHandles.Add(NewVM->AddFieldValueChangedDelegate(ULoadGameSlotViewModel::FFieldNotificationClassDescriptor::bIsSelected, INotifyFieldValueChanged::FFieldValueChangedDelegate::CreateUObject(this, &ThisClass::NotifyFromSelectedSlot)));
		NewVM->SetSavePlayer(SavePlayer);
	}

	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(mLoadGameSlotVMs);
}

/************************
   UOptionSlotViewModel
*************************/

void UOptionSlotViewModel::Select(bool bIsWillSelected)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(bIsSelected, bIsWillSelected))
	{
	}
}

bool UOptionSlotViewModel::IsSelected() const
{
	return bIsSelected;
}

/************************
   UOptionKeyViewModel
*************************/

void UOptionKeyViewModel::SetDisplayName(const FText& DisplayName)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mDisplayName, DisplayName))
	{
	}
}

const FText& UOptionKeyViewModel::GetDisplayName() const
{
	return mDisplayName;
}

void UOptionKeyViewModel::SetMappingName(const FText& MappingName)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mMappingName, MappingName))
	{
	}
}

const FText& UOptionKeyViewModel::GetMappingName() const
{
	return mMappingName;
}

void UOptionKeyViewModel::SetKey(const FKey& Key)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mKey, Key))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetSelectedKey);
		auto* SaveGameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
		if (IsValid(SaveGameSubsystem) == false)
		{
			return;
		}
		FName MappingName = *mMappingName.ToString();
		FEnhancedInputKeyInfo KeyInfo{ *mMappingName.ToString(), *mDisplayName.ToString(), mKey };
		SaveGameSubsystem->GetUserGameSettingTracker()->ApplyCommand<FUserEnhancedInputSettingCommand>(MappingName, KeyInfo);
	}
}

const FKey& UOptionKeyViewModel::GetKey() const
{
	return mKey;
}

FInputChord UOptionKeyViewModel::GetSelectedKey() const
{
	FInputChord SelectedKey = mKey;
	return SelectedKey;
}

/************************
	UOptionViewModel
*************************/

void UOptionViewModel::SetSelectedSlotVM(UOptionSlotViewModel* SelectedSlotVM)
{
	if (IsValid(mSelectedSlotVM) == true && mSelectedSlotVM->IsSelected() == true)
	{
		mSelectedSlotVM->Select(false);
	}

	if (UE_MVVM_SET_PROPERTY_VALUE(mSelectedSlotVM, SelectedSlotVM))
	{
	}
}

UOptionSlotViewModel* UOptionViewModel::GetSelectedSlotVM() const
{
	return mSelectedSlotVM;
}

void UOptionViewModel::SetState(EOptionState State)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mState, State))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetOptionSwitchIndex);
	}
}

void UOptionViewModel::SetScreenResolution(const FString& ScreenResolution)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mScreenResolution, ScreenResolution))
	{
		FString Lhs, Rhs;
		if (ScreenResolution.Split(TEXT("x"), &Lhs, &Rhs) == true)
		{
			auto* SaveGameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
			if (IsValid(SaveGameSubsystem) == false)
			{
				return;
			}

			SaveGameSubsystem->GetUserGameSettingTracker()->ApplyCommand<FUserScreenResolutionSettingCommand>(TEXT("ScreenResolution"), { FCString::Atoi(*Lhs), FCString::Atoi(*Rhs) });
		}
	}
}

void UOptionViewModel::SetMasterVolume(float MasterVolume)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mMasterVolume, MasterVolume))
	{
		auto* SaveGameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
		if (IsValid(SaveGameSubsystem) == false)
		{
			return;
		}
		SaveGameSubsystem->GetUserGameSettingTracker()->ApplyCommand<FUserSoundVolumeSettingCommand>(*EnumToString(ESoundSettingType::Master), { ESoundSettingType::Master, MasterVolume });
	}
}

void UOptionViewModel::SetSFXVolume(float SFXVolume)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mSFXVolume, SFXVolume))
	{
		auto* SaveGameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
		if (IsValid(SaveGameSubsystem) == false)
		{
			return;
		}
		SaveGameSubsystem->GetUserGameSettingTracker()->ApplyCommand<FUserSoundVolumeSettingCommand>(*EnumToString(ESoundSettingType::SFX), { ESoundSettingType::SFX, SFXVolume });
	}
}

void UOptionViewModel::SetVoiceVolume(float VoiceVolume)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mVoiceVolume, VoiceVolume))
	{
		auto* SaveGameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
		if (IsValid(SaveGameSubsystem) == false)
		{
			return;
		}
		SaveGameSubsystem->GetUserGameSettingTracker()->ApplyCommand<FUserSoundVolumeSettingCommand>(*EnumToString(ESoundSettingType::Voice), { ESoundSettingType::Voice, VoiceVolume });
	}
}

void UOptionViewModel::SetBGMVolume(float BGMVolume)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mBGMVolume, BGMVolume))
	{
		auto* SaveGameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
		if (IsValid(SaveGameSubsystem) == false)
		{
			return;
		}
		SaveGameSubsystem->GetUserGameSettingTracker()->ApplyCommand<FUserSoundVolumeSettingCommand>(*EnumToString(ESoundSettingType::BGM), { ESoundSettingType::BGM, BGMVolume });
	}
}

const FString& UOptionViewModel::GetScreenResolution() const
{
	return mScreenResolution;
}

EOptionState UOptionViewModel::GetState() const
{
	return mState;
}

float UOptionViewModel::GetMasterVolume() const
{
	return mMasterVolume;
}

float UOptionViewModel::GetSFXVolume() const
{
	return mSFXVolume;
}

float UOptionViewModel::GetVoiceVolume() const
{
	return mVoiceVolume;
}

float UOptionViewModel::GetBGMVolume() const
{
	return mBGMVolume;
}

int32 UOptionViewModel::GetOptionSwitchIndex() const
{
	return (int32)mState;
}

void UOptionViewModel::NotifyFromSelectedSlot(UObject* Obj, UE::FieldNotification::FFieldId FieIdId)
{
	auto* SlotVM = Cast<UOptionSlotViewModel>(Obj);
	if (IsValid(SlotVM) == true && SlotVM->IsSelected() == true)
	{
		SetSelectedSlotVM(SlotVM);
	}
}

void UOptionViewModel::ConnectSubViewModel(const TArray<UOptionSlotViewModel*>& SubViewModels)
{
	for (auto& SubVM : SubViewModels)
	{
		mOptionSlotVMs.Add(SubVM);
		mSelectedSlotNotifyHandles.Add(SubVM->AddFieldValueChangedDelegate(UOptionSlotViewModel::FFieldNotificationClassDescriptor::bIsSelected, INotifyFieldValueChanged::FFieldValueChangedDelegate::CreateUObject(this, &ThisClass::NotifyFromSelectedSlot)));
	}
}

void UOptionViewModel::Initialize_Implementation()
{
	Super::Initialize_Implementation();

	/* 선택 메뉴 UI */

	const int VMSize = mOptionSlotVMs.Num();
	for (int32 i = 0; i < VMSize; ++i)
	{
		mOptionSlotVMs[i]->RemoveFieldValueChangedDelegate(ULoadGameSlotViewModel::FFieldNotificationClassDescriptor::bIsSelected, mSelectedSlotNotifyHandles[i]);
	}
	if (mOptionSlotVMs.IsEmpty() == false)
	{
		mOptionSlotVMs[0]->Select(true);
	}

	/* 옵션 로드 */

	auto* SaveGameSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(SaveGameSubSystem) == true)
	{
		auto Traker = SaveGameSubSystem->GetUserGameSettingTracker();
		if (IsValid(Traker) == false)
		{
			return;
		}
		const FLinkCommandRow* InputCommandRow = Traker->GetLinkCommands<UUserEnhancedInputSettingLinker>();
		if (InputCommandRow != nullptr)
		{
			for (auto& CommandPair : InputCommandRow->mCommands)
			{
				auto* InputCommand = StaticCast<FUserEnhancedInputSettingCommand*>(CommandPair.Value.Get());
				if (InputCommand != nullptr)
				{
					const FEnhancedInputKeyInfo& KeyInfo = InputCommand->GetCurrenttValue();

					auto* NewVM = NewObject<UOptionKeyViewModel>(this);
					mKeyVMs.Add(NewVM);
					NewVM->SetMappingName(FText::FromName(KeyInfo.mMappingName));
					NewVM->SetDisplayName(FText::FromName(KeyInfo.mDisplayName));
					NewVM->SetKey(KeyInfo.mNewKey);
				}
			}
		}

		SaveGameSubSystem->GetUserGameSettingTracker()->OnCancelSetting.AddUObject(this, &UOptionViewModel::RefreshGameSettingOutfit);
		RefreshGameSettingOutfit();
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(mKeyVMs);
	}
}

void UOptionViewModel::Deinitialize_Implementation()
{
	Super::Deinitialize_Implementation();

	const int VMSize = mOptionSlotVMs.Num();
	for (int32 i = 0; i < VMSize; ++i)
	{
		mOptionSlotVMs[i]->RemoveFieldValueChangedDelegate(ULoadGameSlotViewModel::FFieldNotificationClassDescriptor::bIsSelected, mSelectedSlotNotifyHandles[i]);
	}
	mSelectedSlotNotifyHandles.Empty();
	mOptionSlotVMs.Empty();
}

void UOptionViewModel::SaveSetting()
{
	auto* SaveGameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(SaveGameSubsystem) == false)
	{
		return;
	}
	SaveGameSubsystem->GetUserGameSettingTracker()->SaveSettings();
}

void UOptionViewModel::CancelSetting()
{
	auto* SaveGameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(SaveGameSubsystem) == false)
	{
		return;
	}
	SaveGameSubsystem->GetUserGameSettingTracker()->CancelSettings();
}

void UOptionViewModel::RefreshGameSettingOutfit()
{
	auto* SaveGameSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(SaveGameSubSystem) == true)
	{
		auto Traker = SaveGameSubSystem->GetUserGameSettingTracker();
		if (IsValid(Traker) == false)
		{
			return;
		}
		const FLinkCommandRow* InputCommandRow = Traker->GetLinkCommands<UUserEnhancedInputSettingLinker>();
		if (InputCommandRow != nullptr)
		{
			for (auto& KeyVM : mKeyVMs)
			{
				auto* CommandPtr = InputCommandRow->mCommands.Find(*KeyVM->GetMappingName().ToString());
				if (CommandPtr != nullptr)
				{
					auto* InputCommand = StaticCast<FUserEnhancedInputSettingCommand*>(CommandPtr->Get());
					if (InputCommand != nullptr)
					{
						const FEnhancedInputKeyInfo& KeyInfo = InputCommand->GetCurrenttValue();
						KeyVM->SetDisplayName(FText::FromName(KeyInfo.mDisplayName));
						KeyVM->SetMappingName(FText::FromName(KeyInfo.mMappingName));
						KeyVM->SetKey(KeyInfo.mNewKey);
					}
				}
			}
		}
		const FLinkCommandRow* GraphicCommandRow = Traker->GetLinkCommands<UUserGraphicsSettingLinker>();
		if (GraphicCommandRow != nullptr)
		{
			for (auto& CommandPair : GraphicCommandRow->mCommands)
			{
				auto* ResolutionCommand = StaticCast<FUserScreenResolutionSettingCommand*>(CommandPair.Value.Get());
				if (ResolutionCommand != nullptr)
				{
					const FIntPoint& Resolution = ResolutionCommand->GetCurrenttValue();
					SetScreenResolution(FString::FromInt(Resolution.X) + "x" + FString::FromInt(Resolution.Y));
				}
			}
		}
		const FLinkCommandRow* SoundCommandRow = Traker->GetLinkCommands<UUserSoundSettingLinker>();
		if (SoundCommandRow != nullptr)
		{

			for (auto& CommandPair : SoundCommandRow->mCommands)
			{
				auto* SoundCommand = StaticCast<FUserSoundVolumeSettingCommand*>(CommandPair.Value.Get());
				if (SoundCommand != nullptr)
				{
					switch (SoundCommand->GetCurrenttValue().SettingType)
					{
					case ESoundSettingType::Master:
						SetMasterVolume(SoundCommand->GetCurrenttValue().Volume);
						break;
					case ESoundSettingType::BGM:
						SetBGMVolume(SoundCommand->GetCurrenttValue().Volume);
						break;
					case ESoundSettingType::Voice:
						SetVoiceVolume(SoundCommand->GetCurrenttValue().Volume);
						break;
					case ESoundSettingType::SFX:
						SetSFXVolume(SoundCommand->GetCurrenttValue().Volume);
						break;
					}
				}
			}
		}
	}
}

/************************
	 UTitleViewModel
*************************/

UTitleViewModel::UTitleViewModel() : mState(ETitleState::FirstScreen), bHasPressedKey(false)
{
}

void UTitleViewModel::SetState(ETitleState State)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mState, State))
	{
	}
}

ETitleState UTitleViewModel::GetState() const
{
	return mState;
}

void UTitleViewModel::SetHasPressedKey(bool bIsPressedKey)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(bHasPressedKey, bIsPressedKey))
	{
		SetState(ETitleState::SelectMenu);
	}
}

bool UTitleViewModel::HasPressedKey() const
{
	return bHasPressedKey;
}

bool UTitleViewModel::HasAnySavePlayer() const
{
	auto* SaveGameSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(SaveGameSubSystem) == false)
	{
		return false;
	}

	return SaveGameSubSystem->GetSavePlayers().IsEmpty() == false;
}

void UTitleViewModel::Initialize_Implementation()
{
	Super::Initialize_Implementation();

	auto SaveGameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(SaveGameSubsystem) == false)
	{
		return;
	}
	SaveGameSubsystem->OnChangePlayerSaveList.AddUObject(this, &UTitleViewModel::UpdateTitleMenu);
}

void UTitleViewModel::Deinitialize_Implementation()
{
	Super::Deinitialize_Implementation();
}

void UTitleViewModel::StartNewGame()
{
	SetState(ETitleState::NewGame);

	auto* SaveGameSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(SaveGameSubSystem) == false)
	{
		return;
	}

	auto* NewSavePlayer = NewObject<USavePlayer>(SaveGameSubSystem);
	NewSavePlayer->SetStageName(TEXT("저주받은숲"));
	NewSavePlayer->SetLevel(1);
	NewSavePlayer->SetDate(FDateTime::Now());
	SaveGameSubSystem->SetCurSavePlayer(NewSavePlayer);
	SaveGameSubSystem->SaveAllPlayers(false);
	NewSavePlayer->SetTiggeredEvent(TEXT("StartGame"), false);
}

void UTitleViewModel::ContinueGame()
{
	auto* SaveGameSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(SaveGameSubSystem) == false)
	{
		return;
	}

	auto* HeadNode = SaveGameSubSystem->GetSavePlayers().GetHead();
	if (HeadNode == nullptr)
	{
		return;
	}

	SaveGameSubSystem->SetCurSavePlayer(HeadNode->GetValue()->Clone());
}

void UTitleViewModel::LoadGame()
{
	SetState(ETitleState::LoadGame);
}

void UTitleViewModel::ConfigSettings()
{
	SetState(ETitleState::OptionGame);
}

void UTitleViewModel::EndGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UTitleViewModel::MoveToLoadingMap()
{
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

void UTitleViewModel::UpdateTitleMenu(const TDoubleLinkedList<TObjectPtr<USavePlayer>>& PlayerSaveList)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(HasAnySavePlayer);
}
