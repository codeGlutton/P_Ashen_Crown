#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "GameFramework/Pawn.h"
#include "TitlePawn.generated.h"

/**
 * 타이틀에서 사용할 폰
 */
UCLASS()
class P_ASHEN_CROWN_API ATitlePawn : public APawn
{
	GENERATED_BODY()
	
public:
	ATitlePawn();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "TitlePawn")
	TObjectPtr<class UInputMappingContext> mSettingInputContext;
	UPROPERTY(EditDefaultsOnly, Category = "TitlePawn")
	TSubclassOf<class UUserWidget> mTitleWidgetClass;
};
