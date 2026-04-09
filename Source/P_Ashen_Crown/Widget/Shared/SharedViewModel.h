#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/Widget/ComposableViewModelBase.h"
#include "SharedViewModel.generated.h"

DECLARE_DELEGATE(FOnClosePopupScreen);
DECLARE_DELEGATE_OneParam(FOnCloseSelectionScreen, bool /* Select Result */);

/************************
 UConfirmDialogueViewModel
*************************/

/**
 * 확인, 취소 버튼으로 이루어진 공용 위젯 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UConfirmDialogueViewModel : public UComposableViewModelBase
{
	GENERATED_BODY()

public:
	/* FieldNotify Getter/Setter */

	UFUNCTION(BlueprintCallable, Category = "ConfirmDialogueViewModel")
	void SetTitle(const FText& Title);
	const FText& GetTitle() const;

	UFUNCTION(BlueprintCallable, Category = "ConfirmDialogueViewModel")
	void SetDetail(const FText& Detail);
	const FText& GetDetail() const;

	UFUNCTION(BlueprintCallable, Category = "ConfirmDialogueViewModel")
	void SetFirstSelection(const FText& FirstSelection);
	const FText& GetFirstSelection() const;

	UFUNCTION(BlueprintCallable, Category = "ConfirmDialogueViewModel")
	void SetSecondSelection(const FText& SecondSelection);
	const FText& GetSecondSelection() const;

public:
	/* 수동 실행 함수 */

	/**
	 * 선택창을 고를 시 호출 함수
	 * @param bSelectFirst 첫번째 선택지 선택 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "ConfirmDialogueViewModel")
	void Select(bool bSelectFirst);
	
public:
	FOnCloseSelectionScreen OnCloseSelectionScreen;

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetTitle", Getter = "GetTitle", BlueprintSetter = "SetTitle", meta = (AllowPrivateAccess))
	FText mTitle;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetDetail", Getter = "GetDetail", BlueprintSetter = "SetDetail", meta = (AllowPrivateAccess))
	FText mDetail;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetFirstSelection", Getter = "GetFirstSelection", BlueprintSetter = "SetFirstSelection", meta = (AllowPrivateAccess))
	FText mFirstSelection;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetSecondSelection", Getter = "GetSecondSelection", BlueprintSetter = "SetSecondSelection", meta = (AllowPrivateAccess))
	FText mSecondSelection;
};

/************************
 UWarningMessageViewModel
*************************/

/**
 * 경고 메세지 공용 위젯 ViewModel
 */
UCLASS()
class P_ASHEN_CROWN_API UWarningMessageViewModel : public UComposableViewModelBase
{
	GENERATED_BODY()

public:
	/* FieldNotify Getter/Setter */

	UFUNCTION(BlueprintCallable, Category = "WarningMessageViewModel")
	void SetTitle(const FText& Title);
	const FText& GetTitle() const;

	UFUNCTION(BlueprintCallable, Category = "WarningMessageViewModel")
	void SetDetail(const FText& Detail);
	const FText& GetDetail() const;

	UFUNCTION(BlueprintCallable, Category = "WarningMessageViewModel")
	void SetSelection(const FText& Selection);
	const FText& GetSelection() const;

public:
	/* 수동 실행 함수 */

	/**
	 * 확인하고 넘어갈 시 호출 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "WarningMessageViewModel")
	void Check();

public:
	FOnClosePopupScreen OnClosePopupScreen;

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetTitle", Getter = "GetTitle", BlueprintSetter = "SetTitle", meta = (AllowPrivateAccess))
	FText mTitle;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetDetail", Getter = "GetDetail", BlueprintSetter = "SetDetail", meta = (AllowPrivateAccess))
	FText mDetail;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter = "SetSelection", Getter = "GetSelection", BlueprintSetter = "SetSelection", meta = (AllowPrivateAccess))
	FText mSelection;
};