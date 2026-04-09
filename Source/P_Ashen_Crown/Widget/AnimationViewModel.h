#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "ComposableViewModelBase.h"
#include "AnimationViewModel.generated.h"

UENUM(BlueprintType)
enum class EWidgetAnimState : uint8
{
	None = 0,
	Ready,
	Finish
};

/**
 * 애니메이션 페이드 인 아웃 상태 파악을 위한 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UAnimationViewModel : public UComposableViewModelBase
{
	GENERATED_BODY()

public:
	UAnimationViewModel();

public:
	/* FieldNotify Getter/Setter */

	UFUNCTION(BlueprintCallable, Category = "AnimationViewModel")
	void SetOpeningAnimState(EWidgetAnimState OpeningAnimState);
	EWidgetAnimState GetOpeningAnimState() const;

	UFUNCTION(BlueprintCallable, Category = "AnimationViewModel")
	void SetClosingAnimState(EWidgetAnimState ClosingAnimState);
	EWidgetAnimState GetClosingAnimState() const;

public:
	/* FieldNotify 함수 */

	UFUNCTION(BlueprintPure, FieldNotify)
	bool CanPlayOpeningAnim() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	bool CanPlayClosingAnim() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	bool IsFinishOpeningAnim() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	bool IsFinishClosingAnim() const;

public:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetOpeningAnimState", Getter = "GetOpeningAnimState", BlueprintSetter = "SetOpeningAnimState", meta = (AllowPrivateAccess))
	EWidgetAnimState mOpeningAnimState;

public:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetClosingAnimState", Getter = "GetClosingAnimState", BlueprintSetter = "SetClosingAnimState", meta = (AllowPrivateAccess))
	EWidgetAnimState mClosingAnimState;
};
