#include "DialogueComponent.h"

#include "Engine/AssetManager.h"
#include "Blueprint/UserWidget.h"

#include "P_Ashen_Crown/TableRow/DialogueTableRow.h"
#include "P_Ashen_Crown/Component/DialogueTargetComponent.h"

UDialogueComponent::UDialogueComponent() : 
	mDialogueHandler(nullptr), 
	mDialogueDTObj(nullptr), 
	mCurDialogueDTRow(nullptr), 
	mPageIndex(-1),
	bIsTalking(false),
	mTargetComponent(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UDialogueComponent::IsTalking() const
{
	return bIsTalking;
}

void UDialogueComponent::RegisterDialogue(IDialogueViewHandler* Handler, UUserWidget* Widget, const FText& DisplayName)
{
	mDialogueHandler.SetObject(Handler->_getUObject());
	mDialogueHandler.SetInterface(Handler);
	mDialogueWidget = Widget;

	mDialogueDTObj = nullptr;
	mCurDialogueDTRow = nullptr;
	mTargetComponent = nullptr;

	mDisplayName = DisplayName;
}

void UDialogueComponent::UnregisterDialogue()
{
	CloseDialogue();
	mDialogueHandler.SetObject(nullptr);
	mDialogueHandler.SetInterface(nullptr);
	mDialogueWidget = nullptr;

	mDialogueDTObj = nullptr;
	mCurDialogueDTRow = nullptr;
	mTargetComponent = nullptr;
}

void UDialogueComponent::OpenDialogue(UDialogueTargetComponent* TargetComp, const FName& DialogueName)
{
	if (IsValid(TargetComp) == false || bIsTalking == true)
	{
		return;
	}
	bIsTalking = true;

	// 새로운 상대와 대화하는 경우 우선 DT 비동기 로드 (메모리에 남아있는 경우, 빠르게 수행)
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(TargetComp->GetDialogueDataTable().ToSoftObjectPath(), [Comp = this, TargetComp, DialogueName]()
		{
			if (IsValid(Comp) == false || IsValid(TargetComp) == false)
			{
				Comp->bIsTalking = false;
				return;
			}
			Comp->mDialogueDTObj = TargetComp->GetDialogueDataTable().Get();
			Comp->DeferredOpenDialogue(TargetComp, DialogueName);
		});
	return;
}

void UDialogueComponent::OpenDialogue(UDialogueTargetComponent* TargetComp)
{
	if (IsValid(TargetComp) == false)
	{
		return;
	}
	OpenDialogue(TargetComp, TargetComp->GetDialogueStartKey());
}

void UDialogueComponent::ContinueDialogue(const FName& DialogueName)
{
	if (IsValid(mDialogueDTObj) == false || mDialogueHandler.GetInterface() == nullptr)
	{
		CloseDialogue();
		return;
	}

	mCurDialogueDTRow = mDialogueDTObj->FindRow<FDialogueTableRow>(DialogueName, TEXT(""));
	if (mCurDialogueDTRow == nullptr)
	{
		CloseDialogue();
		return;
	}
	mPageIndex = -1;
	TurnNextPage();
}

void UDialogueComponent::CloseDialogue()
{
	OnEndDialogue.Broadcast(mTargetComponent);
	if (mDialogueHandler.GetInterface() != nullptr)
	{
		mDialogueHandler->CloseView();
	}
	mDialogueDTObj = nullptr;
	mCurDialogueDTRow = nullptr;
	mTargetComponent = nullptr;
	bIsTalking = false;
}

void UDialogueComponent::TurnNextPage()
{
	++mPageIndex;

	// 마지막 대화 페이지로, 조건에 따라 분기 처리
	if (mPageIndex == mCurDialogueDTRow->mEntries.Num())
	{
		// 사용자가 선택 가능한 선택지 후보
		TArray<FDialogueChoiceViewInfo> UsableChoiceInfos;

		int32 ChoicesCount = mCurDialogueDTRow->mChoices.Num();
		for (int32 i = 0; i < ChoicesCount; ++i)
		{
			// 해당 분기 진입 가능 여부
			bool bIsSuccessed = true;

			auto& Choice = mCurDialogueDTRow->mChoices[i];
			for (auto& DecoratorContext : Choice.mDecoratorContexts)
			{
				// 해당 분기 조건 검사 실패 (진입 불가능)
				if (DecoratorContext.mDecorator == nullptr || DecoratorContext.CanEnterChoice(mTargetComponent->GetOwner(), GetOwner()) == false)
				{
					bIsSuccessed = false;
					break;
				}

				// 해당 분기 조건 중 하나 성공
			}

			if (bIsSuccessed == true && mCurDialogueDTRow->mSelectMode == EDialogueSelectMode::Auto)
			{
				DecideOnChoice(i);
				return;
			}
			UsableChoiceInfos.Add({ Choice.mFlow.mChoiceText, bIsSuccessed });
		}

		if (mCurDialogueDTRow->mSelectMode == EDialogueSelectMode::Auto)
		{
			DecideOnChoice(ChoicesCount);
			return;
		}
		UsableChoiceInfos.Add({ mCurDialogueDTRow->mDefaultFlow.mChoiceText, true });
		mDialogueHandler->SetSpeakerText(mDisplayName);
		mDialogueHandler->SetChoiceTexts(UsableChoiceInfos);
	}
	// 다음 대화문으로
	else
	{
		const auto& Entry = mCurDialogueDTRow->mEntries[mPageIndex];
		for (auto& MidTaskContext : Entry.mMidTaskContexts)
		{
			MidTaskContext.ExecuteTask(mTargetComponent->GetOwner(), GetOwner());
		}

		switch (Entry.mSpeakerSetType)
		{
		case EDialogueSpeakerSetType::Custom:
			mDialogueHandler->SetSpeakerText(Entry.mSpeakerName);
			break;
		case EDialogueSpeakerSetType::PlayerName:
			mDialogueHandler->SetSpeakerText(mDisplayName);
			break;
		case EDialogueSpeakerSetType::ActorName:
			mDialogueHandler->SetSpeakerText(mTargetComponent->GetDisplayName());
			break;
		}
		mDialogueHandler->SetPageText(Entry.mContents);
	}
}

void UDialogueComponent::DecideOnChoice(int32 Index)
{
	int32 ChoicesCount = mCurDialogueDTRow->mChoices.Num();
	FDialogueFlow* FlowPtr = nullptr;
	if (ChoicesCount == Index)
	{
		FlowPtr = &mCurDialogueDTRow->mDefaultFlow;
	}
	else
	{
		FlowPtr = &mCurDialogueDTRow->mChoices[Index].mFlow;
	}

	/* 결과 액션 실행 */
	
	bool bEndDialogue = true;
	for (auto& TaskContext : FlowPtr->mTaskContexts)
	{
		if (TaskContext.ExecuteTask(mTargetComponent->GetOwner(), GetOwner()) == EDialogueResult::Continue)
		{
			bEndDialogue = false;
		}
	}
	if (bEndDialogue == true)
	{
		CloseDialogue();
	}
}

void UDialogueComponent::DeferredOpenDialogue(UDialogueTargetComponent* TargetComp, const FName& DialogueName)
{
	mCurDialogueDTRow = mDialogueDTObj->FindRow<FDialogueTableRow>(DialogueName, TEXT(""));
	if (mCurDialogueDTRow == nullptr)
	{
		bIsTalking = false;
		return;
	}

	if (mDialogueHandler.GetInterface() != nullptr)
	{
		mTargetComponent = TargetComp;
		mPageIndex = -1;
		mDialogueWidget->AddToViewport();
		mDialogueHandler->ShowView(this);
		OnStartDialogue.Broadcast(mTargetComponent);
		TurnNextPage();
	}
}



