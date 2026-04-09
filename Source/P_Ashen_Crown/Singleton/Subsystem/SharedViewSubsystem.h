#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MVVMViewModelBase.h"
#include "SharedViewSubsystem.generated.h"

/**
 * 공유 위젯 서브시스템
 */
UCLASS()
class P_ASHEN_CROWN_API USharedViewSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USharedViewSubsystem();

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	/**
	 * 공유 위젯을 탐색하여 반환하는 함수
	 * @param Key 위젯 키 값
	 * @return 탐색 결과 위젯
	 */
	UFUNCTION(BlueprintCallable, Category = "SharedViewSubsystem")
	UUserWidget* GetSharedWidget(const FName& Key);

	/**
	 * 공유 ViewModel을 탐색하여 반환하는 함수
	 * @param Key ViewModel 키 값
	 * @return 탐색 결과 ViewModel
	 */
	UFUNCTION(BlueprintCallable, Category = "SharedViewSubsystem")
	UMVVMViewModelBase* GetSharedViewModel(const FName& Key);

protected:
	/**
	 * 월드와 수명을 같이하는 공유 View를 생성하는 함수 
	 * @param GameMode 대리자 반환해주는 게임 모드
	 * @param NewPlayer 대리자가 반환해주는 플레이어
	 */
	void CreateSharedWorldViews(AGameModeBase* GameMode, APlayerController* NewPlayer);
	/**
	 * 월드와 수명을 같이하는 공유 View를 제거하는 함수
	 * @param GameMode 대리자 반환해주는 게임 모드
	 * @param NewPlayer 대리자가 반환해주는 플레이어
	 */
	void DestroySharedWorldViews(AGameModeBase* GameMode, AController* Exiting);
	/**
	 * 게임 인스턴스와 수명을 같이하는 공유 View를 생성하는 함수 (ViewModel만 유지, 위젯은 월드 당 생성)
	 * @param World 대리자 반환해주는 월드
	 */
	void CreateSharedGlobalViews(AGameModeBase* GameMode, APlayerController* NewPlayer);
	/**
	 * 월드와 수명을 같이하는 공유 View를 생성하는 함수
	 * @param World 대리자 반환해주는 월드
	 */
	void DestroySharedGlobalViews(UWorld* World);

protected:
	UPROPERTY()
	TMap<FName, TObjectPtr<UUserWidget>> mSharedWidgets;
	UPROPERTY()
	TMap<FName, TSubclassOf<UUserWidget>> mSharedWidgetClasses;
	UPROPERTY()
	TMap<FName, TSubclassOf<UMVVMViewModelBase>> mSharedViewModelClasses;

protected:
	FDelegateHandle mWorldPostLoginHandle;
	FDelegateHandle mWorldPreDestroyHandle;
	FDelegateHandle mGlobalPostLoginHandle;
	FDelegateHandle mGlobalPreDestroyHandle;
};
