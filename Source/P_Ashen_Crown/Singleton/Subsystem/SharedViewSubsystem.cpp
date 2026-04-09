#include "SharedViewSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "MVVMGameSubsystem.h"

#include "P_Ashen_Crown/Setting/SharedViewSettings.h"
#include "P_Ashen_Crown/Setting/ACWorldSettings.h"

#include "Engine/AssetManager.h"

USharedViewSubsystem::USharedViewSubsystem()
{
}

void USharedViewSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency<UMVVMGameSubsystem>();
	Super::Initialize(Collection);

	const auto* SettingCDO = GetDefault<USharedViewSettings>();
	if (IsValid(SettingCDO) == false)
	{
		return;
	}

	auto* MVVMSubsystem = GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>();
	if (IsValid(MVVMSubsystem) == false)
	{
		return;
	}

	for (auto& Pair : SettingCDO->mSharedViewClasses)
	{
		auto* WidgetClass = Pair.Value.mSharedWidgetClass.LoadSynchronous();
		mSharedWidgetClasses.Emplace(Pair.Key, WidgetClass);
		auto* VMClass = Pair.Value.mSharedViewModelClass.LoadSynchronous();
		mSharedViewModelClasses.Emplace(Pair.Key, VMClass);

		mSharedWidgets.Add(Pair.Key, nullptr);

		auto* ViewModelObj = NewObject<UMVVMViewModelBase>(GetGameInstance(), VMClass);
		if (IsValid(ViewModelObj) == true)
		{
			FMVVMViewModelContext Context;
			Context.ContextClass = VMClass;
			Context.ContextName = Pair.Key;
			MVVMSubsystem->GetViewModelCollection()->AddViewModelInstance(Context, ViewModelObj);
		}
	}
	mWorldPostLoginHandle = FGameModeEvents::OnGameModePostLoginEvent().AddUObject(this, &USharedViewSubsystem::CreateSharedWorldViews);
	mWorldPreDestroyHandle = FGameModeEvents::OnGameModeLogoutEvent().AddUObject(this, &USharedViewSubsystem::DestroySharedWorldViews);
	mGlobalPostLoginHandle = FGameModeEvents::OnGameModePostLoginEvent().AddUObject(this, &USharedViewSubsystem::CreateSharedGlobalViews);
	mGlobalPreDestroyHandle = FWorldDelegates::OnPreWorldFinishDestroy.AddUObject(this, &USharedViewSubsystem::DestroySharedGlobalViews);
}

void USharedViewSubsystem::Deinitialize()
{
	FGameModeEvents::OnGameModePostLoginEvent().Remove(mWorldPostLoginHandle);
	FWorldDelegates::OnPreWorldFinishDestroy.Remove(mWorldPreDestroyHandle);
	FGameModeEvents::OnGameModePostLoginEvent().Remove(mGlobalPostLoginHandle);
	FWorldDelegates::OnPreWorldFinishDestroy.Remove(mGlobalPreDestroyHandle);
	Super::Deinitialize();
}

UUserWidget* USharedViewSubsystem::GetSharedWidget(const FName& Key)
{
	auto Pair = mSharedWidgets.Find(Key);
	return Pair == nullptr ? nullptr : *Pair;
}

UMVVMViewModelBase* USharedViewSubsystem::GetSharedViewModel(const FName& Key)
{
	auto VMClass = mSharedViewModelClasses.Find(Key);
	if (VMClass == nullptr)
	{
		return nullptr;
	}

	auto* MVVMSubsystem = GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>();
	if (IsValid(MVVMSubsystem) == false)
	{
		return nullptr;
	}

	FMVVMViewModelContext Context;
	Context.ContextClass = *VMClass;
	Context.ContextName = Key;
	return MVVMSubsystem->GetViewModelCollection()->FindViewModelInstance(Context);
}

void USharedViewSubsystem::CreateSharedWorldViews(AGameModeBase* GameMode, APlayerController* NewPlayer)
{
	if (GameMode->GetNumPlayers() != 1 || NewPlayer->IsLocalController() == false)
	{
		return;
	}

	auto* MVVMSubsystem = GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>();
	if (IsValid(MVVMSubsystem) == false)
	{
		return;
	}
	auto* WorldSettings = Cast<AACWorldSettings>(GameMode->GetWorld()->GetWorldSettings());
	if (IsValid(WorldSettings) == false)
	{
		return;
	}

	for (auto Pair : WorldSettings->GetWorldSharedViews())
	{
		auto* WidgetClass = Pair.Value.mSharedWidgetClass.LoadSynchronous();
		auto* VMClass = Pair.Value.mSharedViewModelClass.LoadSynchronous();
		mSharedViewModelClasses.Emplace(Pair.Key, VMClass);

		auto* WidgetObj = CreateWidget<UUserWidget>(NewPlayer, WidgetClass);
		if (IsValid(WidgetObj) == true)
		{
			mSharedWidgets.Emplace(Pair.Key, WidgetObj);
		}
		auto* ViewModelObj = NewObject<UMVVMViewModelBase>(GetGameInstance(), VMClass);
		if (IsValid(ViewModelObj) == true)
		{
			FMVVMViewModelContext Context;
			Context.ContextClass = VMClass;
			Context.ContextName = Pair.Key;
			MVVMSubsystem->GetViewModelCollection()->AddViewModelInstance(Context, ViewModelObj);
		}
	}
}

void USharedViewSubsystem::DestroySharedWorldViews(AGameModeBase* GameMode, AController* Exiting)
{
	auto* MVVMSubsystem = GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>();
	if (IsValid(MVVMSubsystem) == false)
	{
		return;
	}
	auto* WorldSettings = Cast<AACWorldSettings>(GameMode->GetWorld()->GetWorldSettings());
	if (IsValid(WorldSettings) == false)
	{
		return;
	}

	for (auto Pair : WorldSettings->GetWorldSharedViews())
	{
		auto VMClass = mSharedViewModelClasses.Find(Pair.Key);
		if (VMClass != nullptr)
		{
			FMVVMViewModelContext Context;
			Context.ContextClass = *VMClass;
			Context.ContextName = Pair.Key;
			MVVMSubsystem->GetViewModelCollection()->RemoveViewModel(Context);
		}

		mSharedWidgets.Remove(Pair.Key);
	}
}

void USharedViewSubsystem::CreateSharedGlobalViews(AGameModeBase* GameMode, APlayerController* NewPlayer)
{
	if (GameMode->GetNumPlayers() != 1 || NewPlayer->IsLocalController() == false)
	{
		return;
	}

	const auto* SettingCDO = GetDefault<USharedViewSettings>();
	if (IsValid(SettingCDO) == false)
	{
		return;
	}

	for (auto& Pair : SettingCDO->mSharedViewClasses)
	{
		auto* WidgetClass = mSharedWidgetClasses.Find(Pair.Key);
		if (WidgetClass != nullptr)
		{
			auto* WidgetObj = CreateWidget<UUserWidget>(NewPlayer, *WidgetClass);
			if (IsValid(WidgetObj) == true)
			{
				mSharedWidgets[Pair.Key] = WidgetObj;
			}
		}
	}
}

void USharedViewSubsystem::DestroySharedGlobalViews(UWorld* World)
{
	const auto* SettingCDO = GetDefault<USharedViewSettings>();
	if (IsValid(SettingCDO) == false)
	{
		return;
	}

	for (auto& Pair : SettingCDO->mSharedViewClasses)
	{
		mSharedWidgets[Pair.Key] = nullptr;
	}
}

