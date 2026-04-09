#include "LoadingViewModel.h"

#include "Engine/AssetManager.h"
#include "P_Ashen_Crown/Setting/ACWorldSettings.h"
#include "P_Ashen_Crown/TableRow/StageData.h"

#include "P_Ashen_Crown/Singleton/Subsystem/SaveGameSubsystem.h"
#include "P_Ashen_Crown/SaveGame/SavePlayer.h"

/************************
	ULoadingViewModel
*************************/

ULoadingViewModel::ULoadingViewModel() : mLoadingImage(nullptr), mLoadingTitle(), mLoadingText(), mTargetMapStr(), mLoadedPackage(nullptr)
{
}

void ULoadingViewModel::SetLoadingImage(UTexture2D* LoadingImage)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mLoadingImage, LoadingImage))
	{
	}
}

UTexture2D* ULoadingViewModel::GetLoadingImage() const
{
	return mLoadingImage;
}

void ULoadingViewModel::SetLoadingTitle(const FText& LoadingTitle)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mLoadingTitle, LoadingTitle))
	{
	}
}

const FText& ULoadingViewModel::GetLoadingTitle() const
{
	return mLoadingTitle;
}

void ULoadingViewModel::SetLoadingText(const FText& LoadingText)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mLoadingText, LoadingText))
	{
	}
}

const FText& ULoadingViewModel::GetLoadingText() const
{
	return mLoadingText;
}

void ULoadingViewModel::Initialize_Implementation()
{
	Super::Initialize_Implementation();

	auto* WorldSettings = Cast<AACWorldSettings>(GetWorld()->GetWorldSettings());
	if (IsValid(WorldSettings) == false)
	{
		return;
	}

	auto* SaveGameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(SaveGameSubsystem) == false)
	{
		return;
	}

	USavePlayer* CurSavePlayer = SaveGameSubsystem->GetCurSavePlayer();
	FName StageName;
	if (IsValid(CurSavePlayer) == false)
	{
		StageName = TEXT("타이틀");
	}
	else
	{
		StageName = *CurSavePlayer->GetStageName();
	}

	auto* StageTableRow = WorldSettings->GetDataTableRow<FStageData>(EDataTableType::Map, StageName);
	if (StageTableRow != nullptr)
	{
		if (StageTableRow->mLoadingImages.IsEmpty() == false)
		{
			int32 LoadingImageIndex = FMath::RandRange(0, StageTableRow->mLoadingImages.Num() - 1);
			auto Handle = UAssetManager::Get().GetStreamableManager().RequestSyncLoad(StageTableRow->mLoadingImages[LoadingImageIndex].ToSoftObjectPath());
			SetLoadingImage(StageTableRow->mLoadingImages[LoadingImageIndex].Get());
		}
		if (StageTableRow->mLoadingTips.IsEmpty() == false)
		{
			int32 TipIndex = FMath::RandRange(0, StageTableRow->mLoadingTips.Num() - 1);
			SetLoadingTitle(StageTableRow->mLoadingTips[TipIndex].mTitle);
			SetLoadingText(StageTableRow->mLoadingTips[TipIndex].mText);
		}

		mTargetMapStr = StageTableRow->mLevel.GetAssetName();
		LoadPackageAsync(StageTableRow->mLevel.ToString(), FLoadPackageAsyncDelegate::CreateUObject(this, &ULoadingViewModel::CompleteLoadingMap), 0, PKG_ContainsMap);
	}

}

void ULoadingViewModel::Deinitialize_Implementation()
{
	Super::Deinitialize_Implementation();
}

void ULoadingViewModel::MoveToTargetMap()
{
	UGameplayStatics::OpenLevel(GetWorld(), *mTargetMapStr);
}

void ULoadingViewModel::CompleteLoadingMap(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
{
	if (Result == EAsyncLoadingResult::Succeeded)
	{
		mLoadedPackage = LoadedPackage;
		OnLoadTargetMap.Broadcast();
	}
}