// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInputData.h"

UPlayerInputData::UPlayerInputData()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/AC/Data/Input/IMC_PlayerContext.IMC_PlayerContext'"));

	if (InputContext.Succeeded())
		mContext = InputContext.Object;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> UIOnlyInputContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/AC/Data/Input/IMC_UIOnlyPlayerContext.IMC_UIOnlyPlayerContext'"));

	if (UIOnlyInputContext.Succeeded())
		mUIOnlyContext = UIOnlyInputContext.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>	MoveAction(TEXT("/Script/EnhancedInput.InputAction'/Game/AC/Data/Input/IA_Move.IA_Move'"));

	if (MoveAction.Succeeded())
		mMove = MoveAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>	LookAction(TEXT("/Script/EnhancedInput.InputAction'/Game/AC/Data/Input/IA_Look.IA_Look'"));

	if (LookAction.Succeeded())
		mCameraLook = LookAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>	RollAction(TEXT("/Script/EnhancedInput.InputAction'/Game/AC/Data/Input/IA_Roll.IA_Roll'"));

	if (RollAction.Succeeded())
		mRoll = RollAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>	SprintAction(TEXT("/Script/EnhancedInput.InputAction'/Game/AC/Data/Input/IA_Sprint.IA_Sprint'"));

	if (SprintAction.Succeeded())
		mSprint = SprintAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>	AttackAction(TEXT("/Script/EnhancedInput.InputAction'/Game/AC/Data/Input/IA_Attack.IA_Attack'"));

	if (AttackAction.Succeeded())
		mAttack = AttackAction.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputAction>	LockOnOffAction(TEXT("/Script/EnhancedInput.InputAction'/Game/AC/Data/Input/IA_LockOnOff.IA_LockOnOff'"));

	if (LockOnOffAction.Succeeded())
		mLockOnOff = LockOnOffAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>	LockSwitchLeftAction(TEXT("/Script/EnhancedInput.InputAction'/Game/AC/Data/Input/IA_LockSwitchLeft.IA_LockSwitchLeft'"));

	if (LockSwitchLeftAction.Succeeded())
		mLockSwitchLeft = LockSwitchLeftAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>	LockSwitchRightAction(TEXT("/Script/EnhancedInput.InputAction'/Game/AC/Data/Input/IA_LockSwitchRight.IA_LockSwitchRight'"));

	if (LockSwitchRightAction.Succeeded())
		mLockSwitchRight = LockSwitchRightAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>	InteractAction(TEXT("/Script/EnhancedInput.InputAction'/Game/AC/Data/Input/IA_Interact.IA_Interact'"));

	if (InteractAction.Succeeded())
		mInteract = InteractAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>	InventoryAction(TEXT("/Script/EnhancedInput.InputAction'/Game/AC/Data/Input/IA_Inventory.IA_Inventory'"));

	if (InventoryAction.Succeeded())
		mInventory = InventoryAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>	SkillAction(TEXT("/Script/EnhancedInput.InputAction'/Game/AC/Data/Input/IA_Skill.IA_Skill'"));

	if (SkillAction.Succeeded())
		mSkill = SkillAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>	HealAction(TEXT("/Script/EnhancedInput.InputAction'/Game/AC/Data/Input/IA_Heal.IA_Heal'"));

	if (HealAction.Succeeded())
		mHeal = HealAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>	PauseAction(TEXT("/Script/EnhancedInput.InputAction'/Game/AC/Data/Input/IA_Pause.IA_Pause'"));

	if (PauseAction.Succeeded())
		mPause = PauseAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>	RemoveTagAction(TEXT("/Script/EnhancedInput.InputAction'/Game/AC/Data/Input/IA_RemoveTagAll.IA_RemoveTagAll'"));

	if (RemoveTagAction.Succeeded())
		mRemoveAllTag = RemoveTagAction.Object;
}

UInputMappingContext* UPlayerInputData::GetInputMappingContext() const
{
	return mContext.Get();
}

UInputMappingContext* UPlayerInputData::GetUIOnlyInputMappingContext() const
{
	return mUIOnlyContext.Get();
}

UInputAction* UPlayerInputData::GetMoveAction() const
{
	return mMove.Get();
}

UInputAction* UPlayerInputData::GetLookAction() const
{
	return mCameraLook.Get();
}

UInputAction* UPlayerInputData::GetRollAction() const
{
	return mRoll.Get();
}

UInputAction* UPlayerInputData::GetSprintAction() const
{
	return mSprint.Get();
}

UInputAction* UPlayerInputData::GetAttackAction() const
{
	return mAttack.Get();
}

UInputAction* UPlayerInputData::GetLockOnOffAction() const
{
	return mLockOnOff.Get();
}

UInputAction* UPlayerInputData::GetLockSwitchLeftAction() const
{
	return mLockSwitchLeft.Get();
}

UInputAction* UPlayerInputData::GetLockSwitchRightAction() const
{
	return mLockSwitchRight.Get();
}

UInputAction* UPlayerInputData::GetInteractAction() const
{
	return mInteract.Get();
}

UInputAction* UPlayerInputData::GetInventoryAction() const
{
	return mInventory.Get();
}

UInputAction* UPlayerInputData::GetSkillAction() const
{
	return mSkill.Get();
}

UInputAction* UPlayerInputData::GetHealAction() const
{
	return mHeal.Get();
}

UInputAction* UPlayerInputData::GetPauseAction() const
{
	return mPause.Get();
}

UInputAction* UPlayerInputData::GetRemoveTagAction() const
{
	return mRemoveAllTag.Get();
}
