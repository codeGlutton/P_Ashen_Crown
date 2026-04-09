#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "Components/ActorComponent.h"
#include "LockOnTargetComponent.generated.h"

class ULockOnTargetComponent;

/************************
	ILockOnViewHandler
*************************/

UINTERFACE(MinimalAPI)
class ULockOnViewHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 락온 가능한 View와 연결해주는 객체 인터페이스
 */
class P_ASHEN_CROWN_API ILockOnViewHandler
{
	GENERATED_BODY()

public:
	/**
	 * 락온 가능 시 함수
	 * @param TargetComp 연결할 컴포넌트
	 */
	virtual void ShowView(ULockOnTargetComponent* const TargetComp) = 0;
	/**
	 * 락온 종료 시 함수
	 */
	virtual void CloseView() = 0;
};

/************************
  ULockOnTargetComponent
*************************/

/**
 * 락온 대상이 될 수 있도록 해주는 컴포넌트
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Abstract)
class P_ASHEN_CROWN_API ULockOnTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULockOnTargetComponent();

public:
	/**
	 * 위젯과 데이터 전송을 위한 매개체 handler를 초기에 등록하는 함수
	 * @param Handler 데이터를 주고받을 Handler 인터페이스
	 * @param Widget 데이터를 보여줄 위젯
	 */
	virtual void RegisterView(ILockOnViewHandler* Handler, UUserWidget* Widget);
	/**
	 * Handler를 제거. 만약 뷰포트에 표기 시, 자동으로 내림
	 */
	virtual void UnregisterView();

public:
	/**
	 * 락온 View를 보여주는 함수
	 * @param Instigator 락온을 킨 플레이어 캐릭터 객체
	 * @param PlayerController 락온을 킨 플레이어 컨트롤러 객체
	 */
	virtual void OpenLockOnView(AActor* Instigator, APlayerController* PlayerController);
	/**
	 * 락온 View를 닫아주는 함수
	 */
	virtual void CloseLockOnView();

protected:
	UPROPERTY()
	TScriptInterface<ILockOnViewHandler> mLockOnHandler;
	UPROPERTY()
	TObjectPtr<UUserWidget> mLockOnWidget;

protected:
	TObjectPtr<AActor> mCurInstigator;
	TObjectPtr<APlayerController> mCurPlayerController;
};
