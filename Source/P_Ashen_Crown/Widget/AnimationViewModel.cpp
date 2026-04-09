#include "AnimationViewModel.h"

UAnimationViewModel::UAnimationViewModel() : mOpeningAnimState(EWidgetAnimState::None), mClosingAnimState(EWidgetAnimState::None)
{
}

void UAnimationViewModel::SetOpeningAnimState(EWidgetAnimState OpeningAnimState)
{
    if (mOpeningAnimState > OpeningAnimState)
    {
        return;
    }

    if (UE_MVVM_SET_PROPERTY_VALUE(mOpeningAnimState, OpeningAnimState))
    {
        UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CanPlayOpeningAnim);
        UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CanPlayClosingAnim);
        UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsFinishOpeningAnim);
    }
}

EWidgetAnimState UAnimationViewModel::GetOpeningAnimState() const
{
    return mOpeningAnimState;
}

void UAnimationViewModel::SetClosingAnimState(EWidgetAnimState ClosingAnimState)
{
    if (mClosingAnimState > ClosingAnimState)
    {
        return;
    }

    if (UE_MVVM_SET_PROPERTY_VALUE(mClosingAnimState, ClosingAnimState))
    {
        UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CanPlayOpeningAnim);
        UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CanPlayClosingAnim);
        UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsFinishClosingAnim);
    }
}

EWidgetAnimState UAnimationViewModel::GetClosingAnimState() const
{
    return mClosingAnimState;
}

bool UAnimationViewModel::CanPlayOpeningAnim() const
{
    return mOpeningAnimState == EWidgetAnimState::Ready && mClosingAnimState != EWidgetAnimState::Finish;
}

bool UAnimationViewModel::CanPlayClosingAnim() const
{
    return mOpeningAnimState == EWidgetAnimState::Finish && mClosingAnimState == EWidgetAnimState::Ready;
}

bool UAnimationViewModel::IsFinishOpeningAnim() const
{
    return mOpeningAnimState == EWidgetAnimState::Finish;
}

bool UAnimationViewModel::IsFinishClosingAnim() const
{
    return mClosingAnimState == EWidgetAnimState::Finish;
}
