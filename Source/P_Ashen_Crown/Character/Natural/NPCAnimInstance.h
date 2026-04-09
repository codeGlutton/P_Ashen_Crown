#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "P_Ashen_Crown/GAS/GASCore.h"
#include "Animation/AnimInstance.h"
#include "NPCAnimInstance.generated.h"

/**
 * NPC의 애님 인스턴스 클래스
 */
UCLASS()
class P_ASHEN_CROWN_API UNPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UFUNCTION()
	void UpdateTags(const FGameplayTag GameplayTag, int32 Count);

protected:
	UPROPERTY(BlueprintReadWrite, Category = "NPCAnimInstance", meta = (DisplayName = "OwnerASC"))
	TObjectPtr<UAbilitySystemComponent> mOwnerASC;

	UPROPERTY(BlueprintReadWrite, Category = "NPCAnimInstance", meta = (DisplayName = "OwnerTagContainer"))
	FGameplayTagContainer mOwnerTagContainer;

	UPROPERTY(BlueprintReadWrite, Category = "NPCAnimInstance", meta = (DisplayName = "MoveVelocity"))
	FVector mMoveVelocity;

	UPROPERTY(BlueprintReadWrite, Category = "NPCAnimInstance", meta = (DisplayName = "ForwardVelocity"))
	float mForwardVelocity;

	UPROPERTY(BlueprintReadWrite, Category = "NPCAnimInstance", meta = (DisplayName = "RightVelocity"))
	float mRightVelocity;
};
