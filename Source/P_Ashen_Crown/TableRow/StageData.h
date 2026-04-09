#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "Engine/DataTable.h"
#include "StageData.generated.h"

USTRUCT(BlueprintType)
struct P_ASHEN_CROWN_API FLoadingTip
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LoadingTip")
	FText mTitle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LoadingTip")
	FText mText;
};

/**
 * 스테이지 UI표기를 위한 DT의 TableRow 구조체
 */
USTRUCT(BlueprintType)
struct P_ASHEN_CROWN_API FStageData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageData")
	FString mName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageData")
	TSoftObjectPtr<UTexture2D> mImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageData")
	TArray<TSoftObjectPtr<UTexture2D>> mLoadingImages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageData")
	TArray<FLoadingTip> mLoadingTips;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageData")
	TSoftObjectPtr<UWorld> mLevel;
};
