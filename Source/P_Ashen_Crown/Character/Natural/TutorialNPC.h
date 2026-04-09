#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/Character/Natural/NPC.h"
#include "TutorialNPC.generated.h"

/**
 * 튜토리얼 NPC
 */
UCLASS()
class P_ASHEN_CROWN_API ATutorialNPC : public ANPC
{
	GENERATED_BODY()
	
public:
	ATutorialNPC();

public:
	virtual void InitByEventTrigger(class UGameEventSubsystem* GameEventSubsystem) override;

protected:
	virtual void BeginPlay() override;

protected:
	/**
	 * 늙은 노인을 처음 본 경우 게임 이벤트에 의해 호출
	 */
	UFUNCTION()
	void ReceieveGameEvent_SeeOldMan(AActor* EventInstigator, AActor* MainTarget);

	/**
	 * 늙은 노인을 처음 만난 경우 게임 이벤트에 의해 호출
	 */
	UFUNCTION()
	void ReceieveGameEvent_MeetOldMan(AActor* EventInstigator, AActor* MainTarget);

	/**
	 * 몬스터를 잡았을 때 호출
	 * @param Monster 몬스터 액터
	 * @param EndPlayReason 액터 소멸 이유
	 */
	UFUNCTION()
	void SuccessHuntMonsterQuest(AActor* Monster, EEndPlayReason::Type EndPlayReason);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "TutorialNPC", meta = (DisplayName = "SeeOldManSequence"))
	TSoftObjectPtr<class ULevelSequence> mSeeOldManSequence;
	
	UPROPERTY(EditDefaultsOnly, Category = "TutorialNPC", meta = (DisplayName = "SeeOldManEventName"))
	FName mSeeOldManEventName;

	UPROPERTY(EditDefaultsOnly, Category = "TutorialNPC", meta = (DisplayName = "MeetOldManSequence"))
	TSoftObjectPtr<class ULevelSequence> mMeetOldManSequence;

	UPROPERTY(EditDefaultsOnly, Category = "TutorialNPC", meta = (DisplayName = "MeetOldManEventName"))
	FName mMeetOldManEventName;

	UPROPERTY(EditDefaultsOnly, Category = "TutorialNPC", meta = (DisplayName = "TutorialMonsterClass"))
	TSoftClassPtr<class ANormalMonster> mTutorialMonsterClass;

	UPROPERTY(EditDefaultsOnly, Category = "TutorialNPC", meta = (DisplayName = "HuntMonsterEventName"))
	FName mHuntMonsterEventName;

	UPROPERTY(EditDefaultsOnly, Category = "TutorialNPC", meta = (DisplayName = "HuntMonsterEventName"))
	FName mRewardFromOldManEventName;
};
