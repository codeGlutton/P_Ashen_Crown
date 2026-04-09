// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Input/PlayerInputData.h"
#include "PlayerAnimInstance.h"

#include "../../GAS/AttributeSet/PlayerAttributeSet.h"
#include "../../GAS/GameplayAbility/GA_Roll.h"
#include "../../GAS/GameplayAbility/GA_StaminaRegen.h"
#include "../../GAS/GameplayAbility/GA_Attack01.h"
#include "../../GAS/GameplayAbility/GA_Attack02.h"
#include "../../GAS/GameplayAbility/GA_Attack03.h"
#include "../../GAS/GameplayAbility/GA_Attack04.h"
#include "../../GAS/GameplayAbility/GA_Skill01.h"
#include "../../GAS/GameplayAbility/GA_Heal.h"

#include "../../GAS/GameplayEffect/GE_LoadHP.h"

#include "TestAttri/TestAttributeComponent.h"
#include "TestAttri/TestStateComponent.h"

#include "../../Component/LockOnSensingComponent.h"
#include "../../Component/InteractionSensingComponent.h"
#include "../../Component/InteractableTargetComponent.h"
#include "../../Component/DialogueComponent.h"
#include "../../Component/DialogueTargetComponent.h"
#include "../../Component/InventoryComponent.h"

#include "../../ACGameLibrary.h"
#include "../../Singleton/Subsystem/SaveGameSubsystem.h"
#include "../../SaveGame/SavePlayer.h"
#include "../../Singleton/Subsystem/SharedViewSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "../../Game/Main/MainPlayerController.h"
#include "../../Widget/Main/PauseViewHandler.h"

#include "../../GAS/Cue/GameplayCueNotify_Effect.h"
#include "../../GAS/Cue/GameplayCueNotify_Skill01.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 800.f, 0.f);
	//GetCharacterMovement()->bUseControllerDesiredRotation = true;

	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/AC/SourceArt/SkeletalMesh/Kwang/SKM_MainCharSkeletalMesh_00.SKM_MainCharSkeletalMesh_00'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMeshAsset(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<UPlayerAnimInstance> Anim(TEXT("/Script/Engine.AnimBlueprint'/Game/AC/Blueprint/Anim/Player/Nyx/ABP_Player.ABP_Player_C'"));

	if (Anim.Succeeded())
		GetMesh()->SetAnimInstanceClass(Anim.Class);

	mAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("PlayerAttributeSet"));
	mAttributeComp = CreateDefaultSubobject<UTestAttributeComponent>(TEXT("TestAttribute"));
	mStateComp = CreateDefaultSubobject<UTestStateComponent>(TEXT("StateComponent"));
	mLockOnComp = CreateDefaultSubobject<ULockOnSensingComponent>(TEXT("LockOnSensingComponent"));
	mInteractionSensingComponent = CreateDefaultSubobject<UInteractionSensingComponent>(TEXT("InteractionSensingComponent"));
	mDialogueComponent = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComponent"));
	mInventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	GetCapsuleComponent()->SetCapsuleHalfHeight(94.f);

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -94.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	mPlayerSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mPlayerSpringArm->SetupAttachment(GetMesh());
	mPlayerSpringArm->SetRelativeLocation(FVector(0.0, 0.0, 160.0));
	mPlayerSpringArm->SetRelativeRotation(FRotator(-10.0, 90.0, 0.0));
	mPlayerSpringArm->TargetArmLength = 250.f;
	mPlayerSpringArm->bUsePawnControlRotation = true;

	mPlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mPlayerCamera->SetupAttachment(mPlayerSpringArm);
	mPlayerCamera->bUsePawnControlRotation = false;

	mInteractionSensingComponent->SetQuerySphereRadius(500.f);
	mInteractionSensingComponent->SetQueryHalfDegree(30.f);
	mInteractionSensingComponent->SetQueryForwardOffset(-50.f);
	mInteractionSensingComponent->SetTeamFilter(FAISenseAffiliationFilter{ true, true, false });

	mDialogueViewName = TEXT("Dialogue");
	mInventoryViewName = TEXT("Inventory");
	mPauseViewName = TEXT("Pause");

	SetGenericTeamId(ETeamType::Player);
	mDisplayName = FText::FromString(TEXT("플레이어"));
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	mDialogueComponent->OnStartDialogue.AddDynamic(this, &APlayerCharacter::SetFocusDialogueActor);
	mDialogueComponent->OnEndDialogue.AddDynamic(this, &APlayerCharacter::ReleaseFocusDialogueActor);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (mInventoryComponent)
	{
		mInventoryComponent->OnChangeEquipItem.AddUObject(this, &APlayerCharacter::HandleEquipChanged);
	}
	
	mPlayerAnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (IsValid(PlayerController))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		const UPlayerInputData* InputData = GetDefault<UPlayerInputData>();

		Subsystem->AddMappingContext(InputData->GetInputMappingContext(), 1);
		Subsystem->AddMappingContext(InputData->GetUIOnlyInputMappingContext(), 0);
	}

	if (IsValid(mASC))
	{
		mASC->RegisterGameplayTagEvent(AC_PlayerState::Player_State_Death.GetTag(), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::OnDeath);
		mASC->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &APlayerCharacter::OnHit);
		mASC->GiveAbility(FGameplayAbilitySpec(UGA_Roll::StaticClass(), 1, 0));

		FGameplayAbilitySpec RegenSpec(UGA_StaminaRegen::StaticClass(), 1, -1);
		FGameplayAbilitySpecHandle Handle = mASC->GiveAbility(RegenSpec);
		mASC->TryActivateAbility(Handle);

		mASC->GiveAbility(FGameplayAbilitySpec(UGA_Attack01::StaticClass(), 1, 3));
		mASC->GiveAbility(FGameplayAbilitySpec(UGA_Attack02::StaticClass(), 1, 4));
		mASC->GiveAbility(FGameplayAbilitySpec(UGA_Attack03::StaticClass(), 1, 5));
		mASC->GiveAbility(FGameplayAbilitySpec(UGA_Attack04::StaticClass(), 1, 6));
		
		mASC->GiveAbility(FGameplayAbilitySpec(UGA_Skill01::StaticClass(), 1, 7));
		
		mASC->GiveAbility(FGameplayAbilitySpec(UGA_Heal::StaticClass(), 1, 8));
	}

	/* 공유 뷰를 통해 View와 ViewModel 초기 등록 */
	auto* SharedViewSubsystem = GetGameInstance()->GetSubsystem<USharedViewSubsystem>();
	if (IsValid(SharedViewSubsystem) == true)
	{
		// Dialogue 관련
		{
			auto* Widget = SharedViewSubsystem->GetSharedWidget(mDialogueViewName);
			auto* Handler = Cast<IDialogueViewHandler>(SharedViewSubsystem->GetSharedViewModel(mDialogueViewName));
			if (IsValid(Widget) == true && Handler != nullptr)
			{
				mDialogueComponent->RegisterDialogue(Handler, Widget, mDisplayName);
			}
		}

		// Inventory 관련
		{
			auto* Widget = SharedViewSubsystem->GetSharedWidget(mInventoryViewName);
			auto* Handler = Cast<IInventoryViewHandler>(SharedViewSubsystem->GetSharedViewModel(mInventoryViewName));
			if (IsValid(Widget) == true && Handler != nullptr)
			{
				auto* ToolTipWidget = CreateWidget<UUserWidget>(GetController<APlayerController>(), mSharedToolTipWidgetClass);
				mInventoryComponent->RegisterInventoryView(Handler, ToolTipWidget);
				Widget->AddToViewport();
			}
		}

		// Pause 관련
		{
			auto* Widget = SharedViewSubsystem->GetSharedWidget(mPauseViewName);
			if (IsValid(Widget) == true)
			{
				Widget->AddToViewport();
			}
		}
	}
	/* HUD 등록 */

	UUserWidget* HUDWidget = CreateWidget(GetWorld(), mHUDWidgetClass);
	if (IsValid(HUDWidget) == false)
	{
		return;
	}
	HUDWidget->AddToViewport(-1);

	auto* SaveGameSubsystem = GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(SaveGameSubsystem) == true)
	{
		TObjectPtr<USavePlayer> CurSavePlayer = SaveGameSubsystem->GetCurSavePlayer();
		if (IsValid(CurSavePlayer) == true)
		{
			/* 인벤토리 로드 */

			auto& EquipItemKeys = CurSavePlayer->GetEquipItemKeys();
			for (int32 i = 0; i < EquipItemKeys.Num(); ++i)
			{
				mInventoryComponent->LoadEquipItem(i, UACGameLibrary::CreateGameItem(GetWorld(), EquipItemKeys[i].mItemType, EquipItemKeys[i].mName));
			}
			auto& InvenItemKeys = CurSavePlayer->GetInventoryItemKeys();
			for (int32 i = 0; i < InvenItemKeys.Num(); ++i)
			{
				mInventoryComponent->LoadInventoryItem(i, UACGameLibrary::CreateGameItem(GetWorld(), InvenItemKeys[i].mItemType, InvenItemKeys[i].mName));
			}

			/* HP 로드 */
			
			if (CurSavePlayer->GetHP() > 0.f)
			{
				FGameplayEffectContextHandle ContextHandle = mASC->MakeEffectContext();
				FGameplayEffectSpecHandle SpecHandle = mASC->MakeOutgoingSpec(UGE_LoadHP::StaticClass(), 1.f, ContextHandle);

				if (SpecHandle.IsValid())
				{
					SpecHandle.Data->SetSetByCallerMagnitude(AC_PlayerState::Data_LoadData_HP, CurSavePlayer->GetHP());
					mASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (IsValid(Input))
	{
		const UPlayerInputData* InputData = GetDefault<UPlayerInputData>();

		Input->BindAction(InputData->GetMoveAction(), ETriggerEvent::Triggered, this, &APlayerCharacter::MoveKey);

		Input->BindAction(InputData->GetLookAction(), ETriggerEvent::Triggered, this, &APlayerCharacter::LookKey);

		Input->BindAction(InputData->GetRollAction(), ETriggerEvent::Started, this, &APlayerCharacter::RollKey);

		Input->BindAction(InputData->GetSprintAction(), ETriggerEvent::Triggered, this, &APlayerCharacter::SprintKey);

		Input->BindAction(InputData->GetSprintAction(), ETriggerEvent::Completed, this, &APlayerCharacter::StopSprintKey);

		Input->BindAction(InputData->GetAttackAction(), ETriggerEvent::Started, this, &APlayerCharacter::AttackKey);
		
		Input->BindAction(InputData->GetLockOnOffAction(), ETriggerEvent::Started, this, &APlayerCharacter::LockOnOffKey);
		
		Input->BindAction(InputData->GetLockSwitchLeftAction(), ETriggerEvent::Started, this, &APlayerCharacter::LockSwitchLeftKey);

		Input->BindAction(InputData->GetLockSwitchRightAction(), ETriggerEvent::Started, this, &APlayerCharacter::LockSwitchRighttKey);

		Input->BindAction(InputData->GetInteractAction(), ETriggerEvent::Started, this, &APlayerCharacter::InteractKey);

		Input->BindAction(InputData->GetInventoryAction(), ETriggerEvent::Started, this, &APlayerCharacter::InventoryKey);
		
		Input->BindAction(InputData->GetSkillAction(), ETriggerEvent::Started, this, &APlayerCharacter::SkillKey);
		
		Input->BindAction(InputData->GetHealAction(), ETriggerEvent::Started, this, &APlayerCharacter::HealKey);

		Input->BindAction(InputData->GetPauseAction(), ETriggerEvent::Started, this, &APlayerCharacter::PauseKey);

		Input->BindAction(InputData->GetRemoveTagAction(), ETriggerEvent::Started, this, &APlayerCharacter::RemoveTagKey);
	}
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	mDialogueComponent->UnregisterDialogue();
	Super::EndPlay(EndPlayReason);
}

bool APlayerCharacter::HasPotion() const
{
	if (!mInventoryComponent)
	{
		return false;
	}

	const auto& Items = mInventoryComponent->GetInventoryItems();
	for (const auto& Item : Items)
	{
		if (Item.IsValid() && Item->mType == EItemType::Potion)
		{
			return true;
		}
	}
	return false;
}

bool APlayerCharacter::ConsumePotion()
{
	if (!mInventoryComponent)
	{
		return false;
	}

	auto& Items = mInventoryComponent->GetInventoryItems();
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].IsValid() && Items[i]->mType == EItemType::Potion)
		{
			return mInventoryComponent->RemoveInventoryItem(i);
		}
	}
	return false;
}

void APlayerCharacter::CharacterMoveStop()
{
	mStateComp->ToggleMovementInput(false);
}

void APlayerCharacter::MoveKey(const FInputActionValue& Value)
{
	FVector2D Axis = Value.Get<FVector2D>();

	InputMove(Axis);
}

void APlayerCharacter::LookKey(const FInputActionValue& Value)
{
	if (mLockOnComp->IsLockOn())
		return;
	
	FVector2D LookDirection = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookDirection.X);
		AddControllerPitchInput(LookDirection.Y);
	}
}

void APlayerCharacter::RollKey(const FInputActionValue& Value)
{
	check(mAttributeComp);
	check(mStateComp);

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(new FGameplayAbilityTargetData());

	FGameplayEventData EventData;
	EventData.EventTag = AC_PlayerState::Player_State_Rolling.GetTag();
	EventData.Instigator = this;
	EventData.Target = this;
	EventData.TargetData = TargetDataHandle;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventData.EventTag, EventData);
}

void APlayerCharacter::SprintKey()
{
	if (!mASC->HasMatchingGameplayTag(AC_PlayerState::Player_State_Death))
	{
		if (mAttributeComp->CheckHasEnoughStamina(5.f) && IsMoving())
		{
			mAttributeComp->ToggleStaminaRegeneration(false);

			GetCharacterMovement()->MaxWalkSpeed = mSprintSpeed;

			mAttributeComp->DecreaseStamina(0.8f);
		}
		else
		{
			StopSprintKey();
		}
	}
}

void APlayerCharacter::StopSprintKey()
{
	GetCharacterMovement()->MaxWalkSpeed = mNormalSpeed;
	mAttributeComp->ToggleStaminaRegeneration(true);
}

void APlayerCharacter::AttackKey()
{
	if (mASC->HasMatchingGameplayTag(AC_PlayerState::Event_AttackInput_Block))
	{
		return;
	}
	
	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(new FGameplayAbilityTargetData());

	FGameplayEventData EventData;
	EventData.Instigator = this;
	EventData.Target = this;
	EventData.TargetData = TargetDataHandle;

	FGameplayTag NextAttackTag;
	
	if (mASC->HasMatchingGameplayTag(AC_PlayerState::Event_ComboWindow_3.GetTag()))
	{
		NextAttackTag = AC_PlayerState::Player_Combat_Attacking4.GetTag();
	}
	else if (mASC->HasMatchingGameplayTag(AC_PlayerState::Event_ComboWindow_2.GetTag()))
	{
		NextAttackTag = AC_PlayerState::Player_Combat_Attacking3.GetTag();
	}
	else if (mASC->HasMatchingGameplayTag(AC_PlayerState::Event_ComboWindow_Open.GetTag()))
	{
		NextAttackTag = AC_PlayerState::Player_Combat_Attacking2.GetTag();
	}
	else
	{
		NextAttackTag = AC_PlayerState::Player_Combat_Attacking1.GetTag();
	}

	EventData.EventTag = NextAttackTag;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventData.EventTag, EventData);
}

bool APlayerCharacter::IsMoving() const
{
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		return MovementComp->Velocity.Size2D() > 3.f && MovementComp->GetCurrentAcceleration() != FVector::ZeroVector;
	}
	return false;
}

void APlayerCharacter::LockOnOffKey()
{
	if (mLockOnComp)
	{
		mLockOnComp->ToggleLockOn();
	}
}

void APlayerCharacter::LockSwitchLeftKey()
{
	if (mLockOnComp)
	{
		mLockOnComp->SwitchLockOnTarget(-1);
	}
}

void APlayerCharacter::LockSwitchRighttKey()
{
	if (mLockOnComp)
	{
		mLockOnComp->SwitchLockOnTarget(1);
	}
}

void APlayerCharacter::InteractKey()
{
	if (IsValid(mInteractionSensingComponent->GetTargetComponent()) == false)
	{
		return;
	}
	mInteractionSensingComponent->GetTargetComponent()->Interact(this);
}

void APlayerCharacter::InventoryKey()
{
	if (IsValid(mInventoryComponent) == false)
	{
		return;
	}
	mInventoryComponent->ToggleInventory();
}

void APlayerCharacter::SkillKey()
{
	// mPlayerAnimInstance->PlaySkill();
	mASC->TryActivateAbilityByClass(UGA_Skill01::StaticClass());
}

void APlayerCharacter::HealKey()
{
	mASC->TryActivateAbilityByClass(UGA_Heal::StaticClass());
}

void APlayerCharacter::PauseKey()
{
	auto* SharedViewSubsystem = GetGameInstance()->GetSubsystem<USharedViewSubsystem>();
	if (IsValid(SharedViewSubsystem) == true)
	{
		auto* Handler = Cast<IPauseViewHandler>(SharedViewSubsystem->GetSharedViewModel(mPauseViewName));
		if (Handler != nullptr)
		{
			Handler->ToggleView();
		}
	}
}

void APlayerCharacter::RemoveTagKey()
{
	FGameplayTagContainer AllTag = mASC->GetOwnedGameplayTags();
	mASC->RemoveLooseGameplayTags(AllTag);
}

void APlayerCharacter::InputMove(const FVector2D& Axis)
{
	check(mStateComp);
 	if (mStateComp->MovementInputEnabled() == false)
	{
		return;
	}

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotator(0, Rotation.Yaw, 0);

		const FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardVector, Axis.Y);
		AddMovementInput(RightVector, Axis.X);
	}
}

void APlayerCharacter::HandleEquipChanged(int32 EquipIndex, TSharedPtr<const FGameItemInfo> ItemInfo, TSharedPtr<const FGameItemInfo> PreItemInfo)
{
	if (mEquipEffectHandles.Contains(EquipIndex))
	{
		mASC->RemoveActiveGameplayEffect(mEquipEffectHandles[EquipIndex]);
		mEquipEffectHandles.Remove(EquipIndex);
	}

	if (ItemInfo.IsValid())
	{
		TSharedPtr<const FGameEquipInfo> Equip = StaticCastSharedPtr<const FGameEquipInfo>(ItemInfo);
		if (Equip.IsValid() && Equip->mEquipItemStatusEffect)
		{
			FGameplayEffectContextHandle ContextHandle = mASC->MakeEffectContext();
			ContextHandle.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = mASC->MakeOutgoingSpec(Equip->mEquipItemStatusEffect, 1.f, ContextHandle);

			if (SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveHandle = mASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				mEquipEffectHandles.Add(EquipIndex, ActiveHandle);
			}
		}
	}
}

void APlayerCharacter::OnHit(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec,
	FActiveGameplayEffectHandle Handle)
{
	if (mPlayerAnimInstance->bIsHit)
		return;

	if (mASC->HasMatchingGameplayTag(AC_PlayerState::Player_State_Hit.GetTag()))
	{
		const FGameplayEffectContextHandle& Context = Spec.GetEffectContext();
		const FHitResult* HitResult = Context.GetHitResult();
		if (!HitResult)
			return;

		FVector ToHitPoint = (HitResult->ImpactPoint - GetActorLocation()).GetSafeNormal();
		FVector LocalDir = GetActorTransform().InverseTransformVectorNoScale(ToHitPoint);

		float X = FMath::Clamp(LocalDir.Y, -1.f, 1.f);
		float Y = FMath::Clamp(LocalDir.X, -1.f, 1.f);

		mPlayerAnimInstance->HitX = X;
		mPlayerAnimInstance->HitY = Y;
		mPlayerAnimInstance->bIsHit = true;
	}
}

void APlayerCharacter::OnDeath(const FGameplayTag Tag, int32 TagCount)
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerDeath"));
	mPlayerAnimInstance->PlayDeath();

	auto* GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), mGameOverWidgetClass);
	if (IsValid(GameOverWidget) == true)
	{
		GameOverWidget->AddToViewport();
	}
}

UTestStateComponent* APlayerCharacter::GetStateComponent() const
{
	return mStateComp;
}

UInventoryComponent* APlayerCharacter::GetInventoryComponent() const
{
	return mInventoryComponent;
}

UPlayerAnimInstance* APlayerCharacter::GetAnimInstance() const
{
	return mPlayerAnimInstance;
}

UAbilitySystemComponent* APlayerCharacter::GetASC() const
{
	return mASC;
}

void APlayerCharacter::SetFocusDialogueActor(UDialogueTargetComponent* TargetComp)
{
	mLockOnComp->SetManualLockOnTarget(TargetComp->GetOwner());
}

void APlayerCharacter::ReleaseFocusDialogueActor(UDialogueTargetComponent* TargetComp)
{
	mLockOnComp->SetManualLockOnTarget(nullptr);
}
