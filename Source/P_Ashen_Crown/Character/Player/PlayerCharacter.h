// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputActionValue.h"
#include "../BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "Interface/PotionProvider.h"
#include "PlayerCharacter.generated.h"

struct FGameItemInfo;
class UInteractionSensingComponent;
class UDialogueComponent;
class UDialogueTargetComponent;
class UInventoryComponent;
class ULockOnSensingComponent;
class UPlayerAnimInstance;
class UPlayerAttributeSet;
class UTestAttributeComponent;
class UTestStateComponent;

USTRUCT(BlueprintType)
struct FPlayerBonusStatus
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "GameEquipInfo")
	float mBonusAttackPower = 0.f;
	UPROPERTY(EditAnywhere, Category = "GameEquipInfo")
	float mBonusDefense = 0.f;
	UPROPERTY(EditAnywhere, Category = "GameEquipInfo")
	float mBonusMoveSpeed = 0.f;
	UPROPERTY(EditAnywhere, Category = "GameEquipInfo")
	float mBonusStamina = 0.f;
};

UCLASS()
class P_ASHEN_CROWN_API APlayerCharacter : public ABaseCharacter, public IPotionProvider
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent>		mPlayerSpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent>			mPlayerCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPlayerAnimInstance>			mPlayerAnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UPlayerAttributeSet>			mAttributeSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULockOnSensingComponent>	mLockOnComp;
	
	/// <summary>
	/// GAS적용 전까지 사용할 Attribute컴포넌트 변수
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTestAttributeComponent>		mAttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTestStateComponent>		mStateComp; 

protected:
	/// <summary>
	/// 상호작용 객체를 탐지하는 컴포넌트
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UInteractionSensingComponent> mInteractionSensingComponent;

	/// <summary>
	/// 대화문 처리 컴포넌트
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UDialogueComponent> mDialogueComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
	FName mDialogueViewName;

protected:
	/// <summary>
	/// 인벤토리 컴포넌트
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UInventoryComponent> mInventoryComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FName mInventoryViewName;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UUserWidget> mSharedToolTipWidgetClass;
	
	UPROPERTY()
	TMap<int32, FActiveGameplayEffectHandle> mEquipEffectHandles;
	
protected:
	/// <summary>
	/// HUD 위젯 클래스
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<class UUserWidget> mHUDWidgetClass;

protected:
	/// <summary>
	/// 게임 오버 위젯 클래스
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "GameOver")
	TSubclassOf<class UUserWidget> mGameOverWidgetClass;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Pause")
	FName mPauseViewName;

protected:
	UPROPERTY(EditAnywhere, Category = "Sprinting")
	float mSprintSpeed = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Sprinting")
	float mNormalSpeed = 500.f;

public:
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * 인벤토리에 있는 포션 확인
	 * @return 남은 포션 유무
	 */
	virtual bool HasPotion() const override;

	/**
	 * 인벤토리에 있는 포션 소모
	 * @return 소모 성공여부
	 */
	virtual bool ConsumePotion() override;

	/**
	 * 노티파이에서 플레이어 움직임 멈춤 제어용
	 */
	void CharacterMoveStop();

public:
	UTestStateComponent* GetStateComponent() const;
	UInventoryComponent* GetInventoryComponent() const;
	UPlayerAnimInstance* GetAnimInstance() const;
	UAbilitySystemComponent* GetASC() const;

protected:
	/// <summary>
	/// 입력 값에 따라 키 이동을 처리합니다.
	/// </summary>
	/// <param name="Value"> 이동에 사용되는 입력 액션 값입니다 </param>
	void MoveKey(const FInputActionValue& Value);

	/// <summary>
	/// 입력 값에 따라 시점(카메라)이동을 처리합니다.
	/// </summary>
	/// <param name="Value"> 카메라 회전에 사용되는 입력 액션 값입니다 </param>
	void LookKey(const FInputActionValue& Value);

	/// <summary>
	/// 입력 값에 따라 구르기 키 동작을 처리합니다.
	/// </summary>
	/// <param name="Value"> 구르기 동작에 사용되는 입력 액션 값입니다 </param>
	void RollKey(const FInputActionValue& Value);

	/// <summary>
	/// 스프린트 키 입력을 처리합니다.
	/// </summary>
	void SprintKey();

	/// <summary>
	/// 스프린트 멈춤 키 입력을 처리합니다.
	/// </summary>
	void StopSprintKey();

	/// <summary>
	/// 공격 키 입력을 처리합니다.
	/// </summary>
	void AttackKey();

	/// <summary>
	/// 플레이어가 이동중인지 확인합니다.
	/// </summary>
	/// <returns></returns>
	bool IsMoving() const;
	
	/// <summary>
	/// 록 온 입력을 처리합니다.
	/// </summary>
	void LockOnOffKey();

	/// <summary>
	/// 록 온 대상 왼쪽 변경 입력을 처리합니다.
	/// </summary>
	void LockSwitchLeftKey();

	/// <summary>
	/// 록 온 대상 오른쪽 변경 입력을 처리합니다.
	/// </summary>
	void LockSwitchRighttKey();

	/// <summary>
	/// 입력 값에 따라 상호작용 동작을 처리합니다.
	/// </summary>
	void InteractKey();

	/// <summary>
	/// 인벤토리 열고 닫기 입력을 처리합니다.
	/// </summary>
	void InventoryKey();

	/// <summary>
	/// 스킬 입력을 처리합니다.
	/// </summary>
	void SkillKey();
	
	/// <summary>
	/// 체력회복 입력을 처리합니다.
	/// </summary>
	void HealKey();

	/// <summary>
	/// 게임 일시정지 입력을 처리합니다.
	/// </summary>
	void PauseKey();
	
	/// <summary>
	/// Tag가 사라지지않는 버그에 걸렸을 때 도움을 주는 입력을 처리합니다.
	/// </summary>
	void RemoveTagKey();

protected:
	/// <summary>
	/// 캐릭터마다 이동방식이 다를수도 있어서 이동부분만 가상 함수 처리합니다.
	/// </summary>
	/// <param name="Axis"> 이동 방향과 크기를 나타내는 2차원 벡터입니다 </param>
	virtual void InputMove(const FVector2D& Axis);

	/**
	 * 인벤토리 아이템 교체 바인딩
	 * @param EquipIndex 인덱스
	 * @param ItemInfo 새로 Equip한 아이템 정보
	 * @param PreItemInfo 이전 Equip했던 아이템 정보
	 */
	void HandleEquipChanged(int32 EquipIndex, TSharedPtr<const FGameItemInfo> ItemInfo, TSharedPtr<const FGameItemInfo> PreItemInfo);

public:
	void OnHit(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle);
	void OnDeath(const FGameplayTag Tag, int32 TagCount);
	
private:
	FPlayerBonusStatus mPlayerBonusStatus;

	UFUNCTION()
	void SetFocusDialogueActor(UDialogueTargetComponent* TargetComp);
	
	UFUNCTION()
	void ReleaseFocusDialogueActor(UDialogueTargetComponent* TargetComp);
};
