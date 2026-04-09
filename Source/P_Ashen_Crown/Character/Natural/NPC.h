#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/Character/BaseCharacter.h"
#include "P_Ashen_Crown/GAS/GameplayAbility/GA_ChangeEmotion.h"
#include "NPC.generated.h"

/**
 * NPC 캐릭터 객체
 */
UCLASS(Abstract, Blueprintable)
class P_ASHEN_CROWN_API ANPC : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	ANPC();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * 로드된 이벤트 트리거에 따라서 초기화 처리 함수 (BeginPlay에서 호출됨)
	 * @param GameEventSubsystem 이벤트 서브시스템 (nullptr 아님이 보장)
	 */
	virtual void InitByEventTrigger(class UGameEventSubsystem* GameEventSubsystem) {}

public:
	class UBehaviorTree* GetBehaviorTree() const;
	class UBlackboardData* GetBlackBoardData() const;
	virtual void SetDisplayName(const FText& DisplayName) override;

protected:
	/**
	 * 인터랙션 시작 시마다 대화 대상을 향해 회전하도록 명령하는 함수
	 * @param DialogueInstigator 대화 대상
	 */
	UFUNCTION()
	void TurnToDialogueTarget(AActor* DialogueInstigator);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<USpringArmComponent> mSpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<UCameraComponent> mCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<class UDialogueTargetComponent> mDialogueTargetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<class ULockOnTargetCameraComponent> mLockOnTargetCameraComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	FName mInteractableViewName;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<class UBehaviorTree> mBehaviorTree;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<class UBlackboardData> mBlackboardData;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC", meta = (DisplayName = "Emotion Gameplay Abilities"))
	TArray<TSubclassOf<UGA_ChangeEmotion>> mEmotionGAs;
};
