#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "Engine/DeveloperSettings.h"
#include "MVVMViewModelBase.h"
#include "SharedViewSettings.generated.h"

/**
 * MVVM의 View와 ViewModel 클래스 정보
 */
USTRUCT(BlueprintType)
struct FMVVMViewInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FMVVMViewInfo")
    TSoftClassPtr<UUserWidget> mSharedWidgetClass;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FMVVMViewInfo")
    TSoftClassPtr<UMVVMViewModelBase> mSharedViewModelClass;
};

/**
 * 게임 내에 항상 존재할 Shared View 세팅 클래스
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "SharedViewSetting"))
class P_ASHEN_CROWN_API USharedViewSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
    /* Delveloper Setting 함수들 */

    virtual FName GetCategoryName() const override;

#if WITH_EDITOR
    virtual FText GetSectionText() const override;
    virtual FText GetSectionDescription() const override;
#endif

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "SharedView | SharedViewSettings")
    TMap<FName, FMVVMViewInfo> mSharedViewClasses;
};
