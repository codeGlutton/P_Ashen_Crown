#include "DialogueTargetComponent.h"
#include "P_Ashen_Crown/Component/DialogueComponent.h"

UDialogueTargetComponent::UDialogueTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDialogueTargetComponent::SetDialogueDT(const TSoftObjectPtr<UDataTable>& DialogueDT)
{
	mDialogueDT = DialogueDT;
}

void UDialogueTargetComponent::SetDialogueStartKey(const FName& DialogueStartKey)
{
	mDialogueStartKey = DialogueStartKey;
}

void UDialogueTargetComponent::OpenInteractView(AActor* Instigator)
{
	FText DisplayName = mDisplayText;
	mDisplayText = FText::Format(FText::FromString(TEXT("{0} : 대화하기")), mDisplayText);
	Super::OpenInteractView(Instigator);
	mDisplayText = DisplayName;
}

void UDialogueTargetComponent::CloseInteractView()
{
	Super::CloseInteractView();
}

void UDialogueTargetComponent::Interact(UObject* Payload)
{
	if (IsValid(mCurInstigator) == false)
	{
		return;
	}
	auto* DialogueComp = mCurInstigator->FindComponentByClass<UDialogueComponent>();
	if (IsValid(DialogueComp) == false)
	{
		return;
	}
	Super::Interact(Payload);
	DialogueComp->OpenDialogue(this);
}

const TSoftObjectPtr<UDataTable>& UDialogueTargetComponent::GetDialogueDataTable() const
{
	return mDialogueDT;
}

const FName& UDialogueTargetComponent::GetDialogueStartKey() const
{
	return mDialogueStartKey;
}

const FText& UDialogueTargetComponent::GetDisplayName() const
{
	return mDisplayText;
}

