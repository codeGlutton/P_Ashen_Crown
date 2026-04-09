#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "UObject/NoExportTypes.h"
#include "DialogueTableRow.generated.h"

/************************
UDialogueChoiceDecoratorParam
*************************/

/**
 * 선택이 가능한지 조건 검사할 경우 사용되는 파라미터 데이터 에셋
 */
UCLASS(Blueprintable, Abstract)
class P_ASHEN_CROWN_API UDialogueChoiceDecoratorParam : public UDataAsset
{
	GENERATED_BODY()
};

/************************
 UDialogueChoiceDecorator
*************************/

/**
 * 선택문 진입 전에 조건 검사하는 객체 (CDO 사용)
 */
UCLASS(Blueprintable, Abstract)
class P_ASHEN_CROWN_API UDialogueChoiceDecorator : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 해당 대화문에 진입할 수 있는지 여부를 확인하는 함수
	 * @param Param 조건 파라미터 정보가 담긴 데이터 에셋
	 * @param Questioner 질문자 액터
	 * @param Respondent 응답할 액터
	 * @return 성공 여부
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "DialogueChoiceDecorator")
	bool CanEnterChoice(UDialogueChoiceDecoratorParam* Param, AActor* Questioner, AActor* Respondent);
	virtual bool CanEnterChoice_Implementation(UDialogueChoiceDecoratorParam* Param, AActor* Questioner, AActor* Respondent) { return true; };
};

/************************
FDialogueResultTaskContext
*************************/

/**
 * 선택이 가능할지 조건 검사 데이터 집합
 */
USTRUCT(BlueprintType)
struct FDialogueChoiceDecoratorContext
{
	GENERATED_BODY()

public:
	/**
	 * 해당 대화문에 진입할 수 있는지 여부를 확인하는 함수
	 * @param Questioner 질문자 액터
	 * @param Respondent 응답할 액터
	 * @return 성공 여부
	 */
	bool CanEnterChoice(AActor* Questioner, AActor* Respondent);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueChoiceDecoratorContext", meta = (DisplayName = "조건 클래스"))
	TSubclassOf<UDialogueChoiceDecorator> mDecorator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueChoiceDecoratorContext", meta = (DisplayName = "파라미터 데이터 에셋"))
	TSoftObjectPtr<UDialogueChoiceDecoratorParam> mParam;
};

/************************
   UDialogueResultParam
*************************/

/**
 * 선택에 따라 실행할 동작 파라미터 데이터 에셋
 */
UCLASS(Blueprintable, Abstract)
class P_ASHEN_CROWN_API UDialogueResultParam : public UDataAsset
{
	GENERATED_BODY()
};

/************************
   UDialogueResultTask
*************************/

/**
 * 대화 지속 여부 열거형
 */
UENUM(BLueprintType)
enum class EDialogueResult : uint8
{
	Continue = 0,
	End
};

/**
 * 선택에 따라 실행할 동작 구현 객체 (CDO 사용)
 */
UCLASS(Blueprintable, Abstract)
class P_ASHEN_CROWN_API UDialogueResultTask : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 선택에 따라 실행될 결과 동작 함수
	 * @param Param 동작 파라미터 정보가 담긴 데이터 에셋
	 * @param Questioner 질문자 액터
	 * @param Respondent 응답할 액터
	 * @return 대화 지속 여부
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "DialogueResultTask")
	EDialogueResult ExecuteTask(UDialogueResultParam* Param, AActor* Questioner, AActor* Respondent);
	virtual EDialogueResult ExecuteTask_Implementation(UDialogueResultParam* Param, AActor* Questioner, AActor* Respondent) { return EDialogueResult::End; };
};

/************************
FDialogueResultTaskContext
*************************/

/**
 * 실행할 동작을 위한 데이터 집합
 */
USTRUCT(BlueprintType)
struct FDialogueResultTaskContext
{
	GENERATED_BODY()

public:
	/**
	 * 실행될 결과 동작 함수
	 * @param Questioner 질문자 액터
	 * @param Respondent 응답할 액터
	 * @return 대화 지속 여부
	 */
	EDialogueResult ExecuteTask(AActor* Questioner, AActor* Respondent) const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueResultTaskContext", meta = (DisplayName = "액션 클래스"))
	TSubclassOf<UDialogueResultTask> mTask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueResultTaskContext", meta = (DisplayName = "파라미터 데이터 에셋"))
	TSoftObjectPtr<UDialogueResultParam> mParam;
};

/************************
	 UDialogueMidTask
*************************/

/**
 * 대화문 중도 동작 구현 객체 (CDO 사용)
 */
UCLASS(Blueprintable, Abstract)
class P_ASHEN_CROWN_API UDialogueMidTask : public UDialogueResultTask
{
	GENERATED_BODY()

public:
	/**
	 * 실행될 중도 동작 함수
	 * @param Param 동작 파라미터 정보가 담긴 데이터 에셋
	 * @param Questioner 질문자 액터
	 * @param Respondent 응답할 액터
	 */
	UFUNCTION(Category = "DialogueResultTask")
	void ExecuteMidTask(UDialogueResultParam* Param, AActor* Questioner, AActor* Respondent);
};

/************************
 FDialogueMidTaskContext
*************************/

/**
 * 대화문 중간 동작 실행을 위한 데이터 집합 (대화문 흐름에 영향이 없는 동작)
 */
USTRUCT(BlueprintType)
struct FDialogueMidTaskContext
{
	GENERATED_BODY()

public:
	/**
	 * 실행될 중도 동작 함수
	 * @param Questioner 질문자 액터
	 * @param Respondent 응답할 액터
	 */
	void ExecuteTask(AActor* Questioner, AActor* Respondent) const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueMidTaskContext", meta = (DisplayName = "중도 액션 클래스"))
	TSubclassOf<UDialogueMidTask> mMidTask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueMidTaskContext", meta = (DisplayName = "파라미터 데이터 에셋"))
	TSoftObjectPtr<UDialogueResultParam> mParam;
};

/************************
	 FDialogueFlow
*************************/

/**
 * 선택에 따른 분기 흐름 정보
 */
USTRUCT(BlueprintType)
struct FDialogueFlow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FDialogueFlow", meta = (DisplayName = "선택지 이름"))
	FText mChoiceText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FDialogueFlow", meta = (DisplayName = "액션"))
	TArray<FDialogueResultTaskContext> mTaskContexts;
};

/************************
	 FDialogueChoice
*************************/

/**
 * 선택문 객체
 */
USTRUCT(BlueprintType)
struct FDialogueChoice
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueChoice", meta = (DisplayName = "조건"))
	TArray<FDialogueChoiceDecoratorContext> mDecoratorContexts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueChoice", meta = (DisplayName = "충족 시 흐름"))
	FDialogueFlow mFlow;
};

/************************
	  FDialogueEntry
*************************/

UENUM(BlueprintType)
enum class EDialogueSpeakerSetType : uint8
{
	PlayerName = 0 UMETA(ToolTip = "대화 플레이어 이름 사용"),
	ActorName UMETA(ToolTip = "상대 액터 이름 사용"),
	Custom UMETA(ToolTip = "이름 커스텀으로 지정")
};

/**
 * 대화문 요소 객체
 */
USTRUCT(BlueprintType)
struct FDialogueEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTableRow", meta = (DisplayName = "발언자 이름 표기 방식"))
	EDialogueSpeakerSetType mSpeakerSetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueEntry", meta = (DisplayName = "발언자", EditCondition = "mSpeakerSetType == EDialogueSpeakerSetType::Custom"))
	FText mSpeakerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueEntry", meta = (DisplayName = "내용"))
	FText mContents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueEntry", meta = (DisplayName = "중간 액션", ToolTip = "분기 영향 없는 액션 할당"))
	TArray<FDialogueMidTaskContext> mMidTaskContexts;
};

/************************
	FDialogueTableRow
*************************/

UENUM(BlueprintType)
enum class EDialogueSelectMode : uint8
{
	Choice = 0 UMETA(ToolTip = "선택을 유저에게 맡김"),
	Auto = 1 UMETA(ToolTip = "가장 먼저 가능한 최초 선택지를 고름")
};

/**
 * 대화문 테이블 열 구조체
 */
USTRUCT(BlueprintType)
struct FDialogueTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DialogueTableRow", meta = (DisplayName = "대화문"))
	TArray<FDialogueEntry> mEntries;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTableRow", meta = (DisplayName = "분기 선택 방식"))
	EDialogueSelectMode mSelectMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTableRow", meta = (DisplayName = "추가 분기"))
	TArray<FDialogueChoice> mChoices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTableRow", meta = (DisplayName = "기본 동작 흐름"))
	FDialogueFlow mDefaultFlow;
};
