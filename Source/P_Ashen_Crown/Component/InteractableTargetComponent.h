#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "Components/ActorComponent.h"
#include "InteractableTargetComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, AActor*, Instigator);

/************************
 IInteractableViewHandler
*************************/

UINTERFACE(MinimalAPI)
class UInteractableViewHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 상호작용 가능한 View와 연결해주는 객체 인터페이스
 */
class P_ASHEN_CROWN_API IInteractableViewHandler
{
	GENERATED_BODY()

public:
	/**
	 * 상호작용 가능 시 함수
	 * @param TargetComp 연결할 컴포넌트
	 */
	virtual void ShowView(UInteractableTargetComponent* const TargetComp) = 0;
	/**
	 * 표기 텍스트를 지정하는 함수
	 * @param DisplayText 텍스트 내용
	 */
	virtual void SetDisplayText(const FText& DisplayText) = 0;
	/**
	 * 상호작용 종료 시 함수
	 */
	virtual void CloseView() = 0;
};

/************************
UInteractableTargetComponent
*************************/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Abstract )
class P_ASHEN_CROWN_API UInteractableTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractableTargetComponent();

public:
	/**
	 * 위젯과 데이터 전송을 위한 매개체 handler를 초기에 등록하는 함수
	 * @param Handler 데이터를 주고받을 Handler 인터페이스
	 * @param Widget 데이터를 보여줄 위젯
	 * @param DisplayText 보여줄 텍스트
	 */
	virtual void RegisterView(IInteractableViewHandler* Handler, UUserWidget* Widget, const FText& DisplayText);
	/**
	 * Handler를 제거. 만약 뷰포트에 표기 시, 자동으로 내림
	 */
	virtual void UnregisterView();

	void UpdateDisplayText(const FText& DisplayText);

public:
	/**
	 * 상호작용 버튼 View를 보여주는 함수
	 * @param Instigator 상호작용하는 캐릭터 객체
	 */
	virtual void OpenInteractView(AActor* Instigator);
	/**
	 * 상호작용 버튼 View를 닫아주는 함수
	 */
	virtual void CloseInteractView();
	/**
	 * 상호작용 처리 시 호출
	 * @param Payload 전달 객체
	 */
	virtual void Interact(UObject* Payload = nullptr);

public:
	UPROPERTY(BlueprintAssignable, Category = "InteractableTargetComponent")
	FOnInteract OnPreInteract;

protected:
	UPROPERTY()
	TScriptInterface<IInteractableViewHandler> mInteractableHandler;
	UPROPERTY()
	TObjectPtr<UUserWidget> mInteractWidget;

protected:
	FText mDisplayText;

protected:
	TObjectPtr<AActor> mCurInstigator;
};
