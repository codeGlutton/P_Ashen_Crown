#include "UserGameSettingCommand.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedActionKeyMapping.h"

#include "GameFramework/GameUserSettings.h"

#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

/************************
  FUserSettingCommandBase
*************************/

FUserSettingCommandBase::~FUserSettingCommandBase()
{
}

/************************
UUserEnhancedInputSettingLinker
*************************/

uint32 UUserEnhancedInputSettingLinker::mDirtyCount = 0;

bool UUserEnhancedInputSettingLinker::SaveSettings()
{
	if (mDirtyCount == 0)
	{
		return false;
	}
	mDirtyCount = 0;

	auto* EnhancedInputSubSystem = GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (IsValid(EnhancedInputSubSystem) == true)
	{
		UEnhancedInputUserSettings* UserSettings = EnhancedInputSubSystem->GetUserSettings();
		if (IsValid(UserSettings) == true)
		{
			UserSettings->AsyncSaveSettings();
		}
	}
	return true;
}

void UUserEnhancedInputSettingLinker::LoadSettings(TMap<FName, TUniquePtr<FUserSettingCommandBase>>& Commands)
{
	mDirtyCount = 0;

	auto* EnhancedInputSubSystem = GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (IsValid(EnhancedInputSubSystem) == true)
	{
		UEnhancedPlayerMappableKeyProfile* Profile = EnhancedInputSubSystem->GetUserSettings()->GetCurrentKeyProfile();
		for (auto& MappingRows : Profile->GetPlayerMappingRows())
		{
			for (auto& Mapping : MappingRows.Value.Mappings)
			{
				FEnhancedInputKeyInfo KeyInfo;
				KeyInfo.mMappingName = Mapping.GetMappingName();
				KeyInfo.mDisplayName = *Mapping.GetDisplayName().ToString();
				KeyInfo.mNewKey = Mapping.GetCurrentKey();
				Commands.Add(Mapping.GetMappingName(), MakeUnique<FUserEnhancedInputSettingCommand>(KeyInfo, this));
			}
		}
	}

	// 추가적인 세팅 커맨드 생성
}

void UUserEnhancedInputSettingLinker::ChangeInput(const FEnhancedInputKeyInfo& KeySettingInfo)
{
	auto* EnhancedInputSubSystem = GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (IsValid(EnhancedInputSubSystem) == true)
	{
		UEnhancedInputUserSettings* UserSettings = EnhancedInputSubSystem->GetUserSettings();
		if (IsValid(UserSettings) == true)
		{
			FMapPlayerKeyArgs Args;
			Args.MappingName = KeySettingInfo.mMappingName;
			Args.Slot = EPlayerMappableKeySlot::First;
			Args.NewKey = KeySettingInfo.mNewKey;
			FGameplayTagContainer TagContainer;
			UserSettings->MapPlayerKey(Args, TagContainer);
		}
	}
}

/************************
FUserEnhancedInputSettingCommand
*************************/

void FUserEnhancedInputSettingCommand::ActivateCommand()
{
	LinkType* Linker = Cast<LinkType>(mLinker);
	if (Linker != nullptr)
	{
		Linker->ChangeInput(mCurrentValue);
	}
}

/************************
  UUserSoundSettingLinker
************************/

uint32 UUserSoundSettingLinker::mDirtyCount = 0;

UUserSoundSettingLinker::UUserSoundSettingLinker()
{
	mSoundClasses.Init(nullptr, StaticCast<int32>(ESoundSettingType::Count));

	static ConstructorHelpers::FObjectFinder<USoundMix> SCM_Main(TEXT("/Script/Engine.SoundMix'/Game/AC/Blueprint/Sound/SCM_Main.SCM_Main'"));
	if (SCM_Main.Succeeded() == true)
	{
		mSoundMix = SCM_Main.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> SC_Master(TEXT("/Script/Engine.SoundClass'/Game/AC/Blueprint/Sound/SC_Master.SC_Master'"));
	if (SC_Master.Succeeded() == true)
	{
		mSoundClasses[StaticCast<int32>(ESoundSettingType::Master)] = SC_Master.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> SC_SFX(TEXT("/Script/Engine.SoundClass'/Game/AC/Blueprint/Sound/SC_SFX.SC_SFX'"));
	if (SC_SFX.Succeeded() == true)
	{
		mSoundClasses[StaticCast<int32>(ESoundSettingType::SFX)] = SC_SFX.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> SC_Voice(TEXT("/Script/Engine.SoundClass'/Game/AC/Blueprint/Sound/SC_Voice.SC_Voice'"));
	if (SC_Voice.Succeeded() == true)
	{
		mSoundClasses[StaticCast<int32>(ESoundSettingType::Voice)] = SC_Voice.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> SC_BGM(TEXT("/Script/Engine.SoundClass'/Game/AC/Blueprint/Sound/SC_BGM.SC_BGM'"));
	if (SC_BGM.Succeeded() == true)
	{
		mSoundClasses[StaticCast<int32>(ESoundSettingType::BGM)] = SC_BGM.Object;
	}
}

bool UUserSoundSettingLinker::SaveSettings()
{
	if (mDirtyCount == 0)
	{
		return false;
	}
	mDirtyCount = 0;

	UGameplayStatics::SaveGameToSlot(mCurSaveSound, mSaveSoundFileName, 0);
	return true;
}

void UUserSoundSettingLinker::LoadSettings(TMap<FName, TUniquePtr<FUserSettingCommandBase>>& Commands)
{
	mDirtyCount = 0;

	if (mSoundMix == nullptr)
	{
		return;
	}

	UGameplayStatics::PushSoundMixModifier(GetWorld(), mSoundMix);

	mCurSaveSound = Cast<USaveSound>(UGameplayStatics::LoadGameFromSlot(mSaveSoundFileName, 0));
	if (mCurSaveSound == nullptr)
	{
		mCurSaveSound = NewObject<USaveSound>(this);
		uint8 SoundTypeInt = 0;
		for (auto& SoundClass : mSoundClasses)
		{
			ESoundSettingType SoundType = StaticCast<ESoundSettingType>(SoundTypeInt);
			FSoundSettingInfo SoundSettingInfo{ SoundType, 1.f};
			Commands.Add(*EnumToString(SoundType), MakeUnique<FUserSoundVolumeSettingCommand>(SoundSettingInfo, this));
			UGameplayStatics::SetSoundMixClassOverride(GetWorld(), mSoundMix, SoundClass, 1.f);

			++SoundTypeInt;
		}
	}
	else
	{
		uint8 SoundTypeInt = 0;
		for (int32 i = 0; i < mSoundClasses.Num(); ++i)
		{
			ESoundSettingType SoundType = StaticCast<ESoundSettingType>(SoundTypeInt);
			FSoundSettingInfo SoundSettingInfo{ SoundType, mCurSaveSound->GetVolumes()[i] };
			Commands.Add(*EnumToString(SoundType), MakeUnique<FUserSoundVolumeSettingCommand>(SoundSettingInfo, this));
			UGameplayStatics::SetSoundMixClassOverride(GetWorld(), mSoundMix, mSoundClasses[i], mCurSaveSound->GetVolumes()[i]);

			++SoundTypeInt;
		}
	}
}

void UUserSoundSettingLinker::ChangeSoundVolume(const FSoundSettingInfo& SoundSettingInfo)
{
	if (IsValid(mCurSaveSound) == true)
	{
		if (mSoundMix == nullptr)
		{
			return;
		}

		mCurSaveSound->SetVolume(SoundSettingInfo.SettingType, SoundSettingInfo.Volume);
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), mSoundMix, mSoundClasses[StaticCast<int32>(SoundSettingInfo.SettingType)], SoundSettingInfo.Volume);
	}
}

/************************
UUserGraphicsSettingLinker
*************************/

uint32 UUserGraphicsSettingLinker::mDirtyCount = 0;

bool UUserGraphicsSettingLinker::SaveSettings()
{
	if (mDirtyCount == 0)
	{
		return false;
	}
	mDirtyCount = 0;

	UGameUserSettings::GetGameUserSettings()->SaveSettings();
	return true;
}

void UUserGraphicsSettingLinker::LoadSettings(TMap<FName, TUniquePtr<FUserSettingCommandBase>>& Commands)
{
	mDirtyCount = 0;

	auto* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	if (IsValid(GameUserSettings) == true)
	{
		Commands.Add(TEXT("ScreenResolution"), MakeUnique<FUserScreenResolutionSettingCommand>(GameUserSettings->GetScreenResolution(), this));
	}

	// 추가적인 세팅 커맨드 생성
}

void UUserGraphicsSettingLinker::ChangeScreenResolution(FIntPoint Resolution)
{
	auto* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	if (IsValid(GameUserSettings) == true)
	{
		GameUserSettings->SetScreenResolution(Resolution);
		GameUserSettings->ApplyResolutionSettings(false);
	}
}

/************************
FUserSoundSettingCommand
*************************/

void FUserSoundVolumeSettingCommand::ActivateCommand()
{
	LinkType* Linker = Cast<LinkType>(mLinker);
	if (Linker != nullptr)
	{
		Linker->ChangeSoundVolume(mCurrentValue);
	}
}

/************************
FUserScreenResolutionSettingCommand
*************************/

void FUserScreenResolutionSettingCommand::ActivateCommand()
{
	LinkType* Linker = Cast<LinkType>(mLinker);
	if (Linker != nullptr)
	{
		Linker->ChangeScreenResolution(mCurrentValue);
	}
}

