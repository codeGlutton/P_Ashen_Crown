#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/Component/InteractableTargetComponent.h"
#include "DialogueTargetComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P_ASHEN_CROWN_API UDialogueTargetComponent : public UInteractableTargetComponent
{
	GENERATED_BODY()

public:	
	UDialogueTargetComponent();

public:
	void SetDialogueDT(const TSoftObjectPtr<UDataTable>& DialogueDT);
	void SetDialogueStartKey(const FName& DialogueStartKey);

public:
	virtual void OpenInteractView(AActor* Instigator) override;
	virtual void CloseInteractView() override;
	virtual void Interact(UObject* Payload = nullptr) override;

public:
	const TSoftObjectPtr<UDataTable>& GetDialogueDataTable() const;
	const FName& GetDialogueStartKey() const;
	const FText& GetDisplayName() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "DialogueTargetComponent", meta = (DisplayName = "특정 대화문 데이터 테이블"))
	TSoftObjectPtr<UDataTable> mDialogueDT;
	UPROPERTY(EditDefaultsOnly, Category = "DialogueTargetComponent", meta = (DisplayName = "첫 대화문 키 값"))
	FName mDialogueStartKey;
};
