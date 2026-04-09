#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "Components/ActorComponent.h"
#include "UObject/Interface.h"
#include "DialogueComponent.generated.h"

class UUserWidget;
class UDialogueComponent;
class UDialogueTargetComponent;
struct FDialogueTableRow;

/************************
  FDialogueChoiceViewInfo
*************************/

USTRUCT()
struct FDialogueChoiceViewInfo
{
	GENERATED_BODY()

public:
	FText mChoiceText;
	bool bIsEnabled;
};

/************************
   IDialogueViewHandler
*************************/

UINTERFACE(MinimalAPI)
class UDialogueViewHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 다이얼로그 View와 연결해주는 객체 인터페이스
 */
class P_ASHEN_CROWN_API IDialogueViewHandler
{
	GENERATED_BODY()

public:
	/**
	 * 상호작용 시작 시 함수
	 * @param DialogueComp 연결할 컴포넌트
	 */
	virtual void ShowView(UDialogueComponent* const DialogueComp) = 0;
	/**
	 * 대화 주체를 지정하는 함수
	 * @param Speaker 대화 주체 이름
	 */
	virtual void SetSpeakerText(const FText& Speaker) = 0;
	/**
	 * 대화문을 지정하는 함수
	 * @param Contents 대화 내용
	 */
	virtual void SetPageText(const FText& Contents) = 0;
	/**
	 * 선택문을 제공하는 함수
	 * @param Choices 선택지들
	 */
	virtual void SetChoiceTexts(const TArray<FDialogueChoiceViewInfo>& ChoiceInfos) = 0;
	/**
	 * 상호작용 종료 시 함수
	 */
	virtual void CloseView() = 0;
};

/************************
    UDialogueComponent
*************************/

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartDialogue, UDialogueTargetComponent*, TargetComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndDialogue, UDialogueTargetComponent*, TargetComp);

/**
 * 대화창을 띄우는 컴포넌트
 */
UCLASS( ClassGroup=(Dialogue), meta=(BlueprintSpawnableComponent) )
class P_ASHEN_CROWN_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDialogueComponent();

public:
	bool IsTalking() const;

public:
	/**
	 * 위젯과 데이터 전송을 위한 매개체 handler를 초기에 등록하는 함수
	 * @param Handler 데이터를 주고받을 Handler 인터페이스
	 * @param Widget 데이터를 보여줄 위젯
	 * @param DisplayName 플레이어 표기 이름
	 */
	void RegisterDialogue(IDialogueViewHandler* Handler, UUserWidget* Widget, const FText& DisplayName);
	/**
	 * Handler를 제거. 만약 뷰포트에 표기 시, 자동으로 내림
	 */
	void UnregisterDialogue();

public:
	/**
	 * 실제 뷰포트에 보여주는 함수
	 * @param TargetComp 대화할 대상 컴포넌트
	 * @param DialogueName 테이블 Row 키 이름
	 */
	void OpenDialogue(UDialogueTargetComponent* TargetComp, const FName& DialogueName);
	void OpenDialogue(UDialogueTargetComponent* TargetComp);
	/**
	 * 이어서 대화문을 보여주는 함수
	 * @param DialogueName 테이블 Row 키 이름
	 */
	void ContinueDialogue(const FName& DialogueName);
	/**
	 * 뷰포트에서 위젯을 제거하는 함수
	 */
	void CloseDialogue();
	/**
	 * 다음 페이지로 넘어가 View를 업데이트하는 함수
	 */
	void TurnNextPage();
	/**
	 * 선택지를 결정함에 따라 Task 내부 로직을 실행하는 함수
	 * @param Index 선택지 인덱스
	 */
	void DecideOnChoice(int32 Index);

protected:
	/*
	 * 실제 뷰포트에 보여주는 콜백 함수
	 * @param TargetComp 대화할 대상 컴포넌트
	 * @param DialogueName 테이블 Row 키 이름
	*/
	void DeferredOpenDialogue(UDialogueTargetComponent* TargetComp, const FName& DialogueName);

public:
	UPROPERTY(BlueprintAssignable, Category = "DialogueComponent")
	FOnStartDialogue OnStartDialogue;
	UPROPERTY(BlueprintAssignable, Category = "DialogueComponent")
	FOnEndDialogue OnEndDialogue;

protected:
	UPROPERTY()
	TScriptInterface<IDialogueViewHandler> mDialogueHandler;
	UPROPERTY()
	TObjectPtr<UUserWidget> mDialogueWidget;

protected:
	FText mDisplayName;

protected:
	// 데이터 테이블 하드 레퍼런싱용
	UPROPERTY()
	TObjectPtr<UDataTable> mDialogueDTObj;
	FDialogueTableRow* mCurDialogueDTRow;
	int32 mPageIndex;
	bool bIsTalking;

protected:
	TObjectPtr<UDialogueTargetComponent> mTargetComponent;
};
