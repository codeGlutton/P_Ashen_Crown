#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/TableRow/DialogueTableRow.h"
#include "GameplayAbilities/Public/AttributeSet.h"
#include "DLDeco_FindGameplayAttribute.generated.h"

UENUM(BlueprintType)
enum class EGameplayAttributeFinderTarget : uint8
{
	Player = 0,
	Speaker
};

UENUM(BlueprintType)
enum class EGameplayAttributeValueComparisonType : uint8
{
	None  UMETA(DisplayName = "In", ToolTip = "단순히 포함 여부만 확인"),
	Equal UMETA(DisplayName = "=="),
	NotEqual UMETA(DisplayName = "!="),
	Greater UMETA(DisplayName = ">"),
	Less UMETA(DisplayName = "<"),
	GreaterOrEqual UMETA(DisplayName = ">="),
	LessOrEqual UMETA(DisplayName = "<="),
};

UCLASS()
class P_ASHEN_CROWN_API UDLDecoParam_FindGameplayAttribute : public UDialogueChoiceDecoratorParam
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="FindGameplayAttribute")
	EGameplayAttributeFinderTarget mTargetAttribute;

	UPROPERTY(EditDefaultsOnly, Category = "FindGameplayAttribute")
	FGameplayAttribute mAtrribute;

	UPROPERTY(EditDefaultsOnly, Category = "FindGameplayAttribute")
	EGameplayAttributeValueComparisonType mValueFilter;

	UPROPERTY(EditDefaultsOnly, Category = "FindGameplayAttribute")
	float mCompareValue;
};

/**
 * 속성이 있는지 여부를 확인하는 Dialogue Decorator
 */
UCLASS()
class P_ASHEN_CROWN_API UDLDeco_FindGameplayAttribute : public UDialogueChoiceDecorator
{
	GENERATED_BODY()
	
public:
	virtual bool CanEnterChoice_Implementation(UDialogueChoiceDecoratorParam* Param, AActor* Questioner, AActor* Respondent) override;
};
