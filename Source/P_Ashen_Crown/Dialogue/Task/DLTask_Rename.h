#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/TableRow/DialogueTableRow.h"
#include "DLTask_Rename.generated.h"

UENUM(BlueprintType)
enum class ERenameTarget : uint8
{
	ActorName = 0,
	PlayerName,
};

UCLASS()
class P_ASHEN_CROWN_API UDLTaskParam_Rename : public UDialogueResultParam
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Rename")
	FText mDisplayName;

	UPROPERTY(EditDefaultsOnly, Category = "Rename")
	ERenameTarget mRenameTarget;
};

/**
 * 이름을 변경하는 Dialogue Task
 */
UCLASS()
class P_ASHEN_CROWN_API UDLTask_Rename : public UDialogueMidTask
{
	GENERATED_BODY()
	
public:
	virtual EDialogueResult ExecuteTask_Implementation(UDialogueResultParam* Param, AActor* Questioner, AActor* Respondent) override;
};
