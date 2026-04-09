// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameCore.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "UObject/NoExportTypes.h"
#include "PlayerInputData.generated.h"

UCLASS()
class P_ASHEN_CROWN_API UPlayerInputData : public UObject
{
	GENERATED_BODY()
	
public:
	UPlayerInputData();

private:
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext>	mContext;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext>	mUIOnlyContext;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction>			mMove;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction>			mCameraLook;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction>			mRoll;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction>			mSprint;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction>			mAttack;
	
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction>			mLockOnOff;
	
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction>			mLockSwitchLeft;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction>			mLockSwitchRight;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction>			mInteract;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction>			mInventory;
	
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction>			mSkill;
	
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction>			mHeal;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction>			mPause;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction>			mRemoveAllTag;

public:
	UInputMappingContext* GetInputMappingContext() const;

	UInputMappingContext* GetUIOnlyInputMappingContext() const;

	UInputAction* GetMoveAction() const;

	UInputAction* GetLookAction() const;

	UInputAction* GetRollAction() const;

	UInputAction* GetSprintAction() const;

	UInputAction* GetAttackAction() const;
	
	UInputAction* GetLockOnOffAction() const;
	
	UInputAction* GetLockSwitchLeftAction() const;

	UInputAction* GetLockSwitchRightAction() const;

	UInputAction* GetInteractAction() const;

	UInputAction* GetInventoryAction() const;
	
	UInputAction* GetSkillAction() const;
	
	UInputAction* GetHealAction() const;

	UInputAction* GetPauseAction() const;

	UInputAction* GetRemoveTagAction() const;
};
