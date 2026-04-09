#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UWidget;

/**
 * 메인 맵에서 사용할 플레이어 컨트롤러
 */
UCLASS()
class P_ASHEN_CROWN_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainPlayerController();

protected:
	virtual void BeginPlay() override;

public:
	/**
	 * 게임 인풋을 복구하고 싶은 경우, UI Only Mode 내부 카운팅 김소
	 */
	void DecreaseUIOnlyModeCount();
	/**
	 * 불필요한 게임 인풋을 제거하고 싶은 경우, UI Only Mode 내부 카운팅 증가
	 * @param FocusWidget 포커스를 줄 Widget
	 */
	void IncreaseUIOnlyModeCount(UWidget* FocusWidget = nullptr);
	/**
	 * 강제로 게임 인풋을 복구하고 싶은 경우 호출하는 함수
	 * UI Only Mode 내부 카운팅을 강제로 0 변환
	 */
	void ClearUIOnlyModeCount();

protected:
	void SetUIOnlyMode(UWidget* FocusWidget);
	void SetGameOnlyMode();

protected:
	int32 mUIOnlyModeCount;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayerController")
	TObjectPtr<class UCameraTargetingBlendComponent> mCameraTargetingBlendComponent;
};
