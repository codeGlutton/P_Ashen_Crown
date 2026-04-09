#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/GAS/GASCore.h"

#include "P_Ashen_Crown/TableRow/DialogueTableRow.h"
#include "DLTask_ExecuteGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EExecuteGameplayAbilityTarget : uint8
{
	ActorASC = 0,
	PlayerASC,
};

UENUM(BlueprintType)
enum class EGameplayAbilityCallMethod : uint8
{
	Tag = 0 UMETA(ToolTip = "태그로 실행"),
	Event UMETA(ToolTip = "이벤트 실행"),
	Class UMETA(ToolTip = "클래스로 실행"),
	InputID UMETA(ToolTip = "Spec의 InputID 값으로 실행")
};

UCLASS()
class P_ASHEN_CROWN_API UDLTaskParam_ExecuteGameplayAbility : public UDialogueResultParam
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "ExecuteGameplayAbility", meta = (DisplayName = "TargetType"))
	EExecuteGameplayAbilityTarget mTargetType;

	UPROPERTY(EditDefaultsOnly, Category = "ExecuteGameplayAbility", meta = (DisplayName = "CallMethod"))
	EGameplayAbilityCallMethod mCallMethod;

	UPROPERTY(EditDefaultsOnly, Category = "ExecuteGameplayAbility", meta = (DisplayName = "EventTag", GameplayTagFilter = "GameplayEventTagsCategory", EditCondition = "mCallMethod == EGameplayAbilityCallMethod::Event"))
	FGameplayTag mEventTag;

	UPROPERTY(EditDefaultsOnly, Category = "ExecuteGameplayAbility", meta = (DisplayName = "InstigatorTags", EditCondition = "mCallMethod == EGameplayAbilityCallMethod::Event"))
	FGameplayTagContainer mInstigatorTags;

	UPROPERTY(EditDefaultsOnly, Category = "ExecuteGameplayAbility", meta = (DisplayName = "TargetTags", EditCondition = "mCallMethod == EGameplayAbilityCallMethod::Tag || mCallMethod == EGameplayAbilityCallMethod::Event"))
	FGameplayTagContainer mTargetTags;

	UPROPERTY(EditDefaultsOnly, Category = "ExecuteGameplayAbility", meta = (DisplayName = "Class", EditCondition = "mCallMethod == EGameplayAbilityCallMethod::Class"))
	TSubclassOf<UGameplayAbility> mClass;

	UPROPERTY(EditDefaultsOnly, Category = "ExecuteGameplayAbility", meta = (DisplayName = "InputID", EditCondition = "mCallMethod == EGameplayAbilityCallMethod::InputID"))
	int32 mInputID;
};

/**
 * 게임플레이 능력을 실행시키는 Dialogue Task
 */
UCLASS()
class P_ASHEN_CROWN_API UDLTask_ExecuteGameplayAbility : public UDialogueMidTask
{
	GENERATED_BODY()

public:
	virtual EDialogueResult ExecuteTask_Implementation(UDialogueResultParam* Param, AActor* Questioner, AActor* Respondent) override;
};
