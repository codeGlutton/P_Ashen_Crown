#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/Widget/ComposableViewModelBase.h"
#include "LoadingViewModel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadMap);

/************************
	ULoadingViewModel
*************************/

/**
 * 로딩 맵 UI의 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API ULoadingViewModel : public UComposableViewModelBase
{
	GENERATED_BODY()
	
public:
	ULoadingViewModel();

public:
	/* FieldNotify Getter/Setter */

	UFUNCTION(BlueprintCallable, Category = "LoadingViewModel")
	void SetLoadingImage(UTexture2D* LoadingImage);
	UTexture2D* GetLoadingImage() const;

	UFUNCTION(BlueprintCallable, Category = "LoadingViewModel")
	void SetLoadingTitle(const FText& LoadingTitle);
	const FText& GetLoadingTitle() const;

	UFUNCTION(BlueprintCallable, Category = "LoadingViewModel")
	void SetLoadingText(const FText& LoadingText);
	const FText& GetLoadingText() const;

public:
	/* 수동 실행 함수 */

	virtual void Initialize_Implementation() override;
	virtual void Deinitialize_Implementation() override;

	/**
	 * 타겟 맵으로 이동
	 */
	UFUNCTION(BlueprintCallable, Category = "LoadingViewModel")
	void MoveToTargetMap();

protected:
	/**
	 * 맵 로딩 완료시에 콜백 함수
	 * @param PackageName 로딩된 다음 맵 이름
	 * @param LoadedPackage 로드된 패키지
	 * @param Result 로드 결과
	 */
	void CompleteLoadingMap(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result);

public:
	UPROPERTY(BlueprintAssignable, Category = "LoadingViewModel")
	FOnLoadMap OnLoadTargetMap;

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetLoadingImage", Getter = "GetLoadingImage", BlueprintSetter = "SetLoadingImage", meta = (AllowPrivateAccess))
	TObjectPtr<UTexture2D> mLoadingImage;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetLoadingTitle", Getter = "GetLoadingTitle", BlueprintSetter = "SetLoadingTitle", meta = (AllowPrivateAccess))
	FText mLoadingTitle;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetLoadingText", Getter = "GetLoadingText", BlueprintSetter = "SetLoadingText", meta = (AllowPrivateAccess))
	FText mLoadingText;

private:
	FString mTargetMapStr;

	UPROPERTY()
	TObjectPtr<UPackage> mLoadedPackage;
};
