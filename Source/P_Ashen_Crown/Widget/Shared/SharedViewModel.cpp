#include "SharedViewModel.h"

/************************
 UConfirmDialogueViewModel
*************************/

void UConfirmDialogueViewModel::SetTitle(const FText& Title)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mTitle, Title))
	{
	}
}

const FText& UConfirmDialogueViewModel::GetTitle() const
{
	return mTitle;
}

void UConfirmDialogueViewModel::SetDetail(const FText& Detail)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mDetail, Detail))
	{
	}
}

const FText& UConfirmDialogueViewModel::GetDetail() const
{
	return mDetail;
}

void UConfirmDialogueViewModel::SetFirstSelection(const FText& FirstSelection)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mFirstSelection, FirstSelection))
	{
	}
}

const FText& UConfirmDialogueViewModel::GetFirstSelection() const
{
	return mFirstSelection;
}

void UConfirmDialogueViewModel::SetSecondSelection(const FText& SecondSelection)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mSecondSelection, SecondSelection))
	{
	}
}

const FText& UConfirmDialogueViewModel::GetSecondSelection() const
{
	return mSecondSelection;
}

void UConfirmDialogueViewModel::Select(bool bSelectFirst)
{
	OnCloseSelectionScreen.ExecuteIfBound(bSelectFirst);
}

/************************
 UWarningMessageViewModel
*************************/

void UWarningMessageViewModel::SetTitle(const FText& Title)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mTitle, Title))
	{
	}
}

const FText& UWarningMessageViewModel::GetTitle() const
{
	return mTitle;
}

void UWarningMessageViewModel::SetDetail(const FText& Detail)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mDetail, Detail))
	{
	}
}

const FText& UWarningMessageViewModel::GetDetail() const
{
	return mDetail;
}

void UWarningMessageViewModel::SetSelection(const FText& Selection)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(mSelection, Selection))
	{
	}
}

const FText& UWarningMessageViewModel::GetSelection() const
{
	return mSelection;
}

void UWarningMessageViewModel::Check()
{
	OnClosePopupScreen.ExecuteIfBound();
}
