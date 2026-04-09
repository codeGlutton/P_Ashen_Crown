#include "NPC.h"
#include "P_Ashen_Crown/Character/Natural/NPCAIController.h"

#include "P_Ashen_Crown/Setting/TeamSettings.h"

#include "Blueprint/UserWidget.h"

#include "P_Ashen_Crown/Component/DialogueComponent.h"
#include "P_Ashen_Crown/Component/DialogueTargetComponent.h"
#include "P_Ashen_Crown/Singleton/Subsystem/SharedViewSubsystem.h"

#include "P_Ashen_Crown/Component/LockOnTargetCameraComponent.h"

#include "P_Ashen_Crown/Singleton/Subsystem/GameEventSubsystem.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"


ANPC::ANPC()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	mSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	mSpringArmComponent->SetupAttachment(GetRootComponent());
	mSpringArmComponent->SetRelativeLocation(FVector(0., 0., 50.));
	mSpringArmComponent->SetRelativeRotation(FRotator(-15., 180., 0.));
	mSpringArmComponent->TargetArmLength = 200.f;

	mCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	mCameraComponent->SetupAttachment(mSpringArmComponent);

	mLockOnTargetCameraComponent = CreateDefaultSubobject<ULockOnTargetCameraComponent>(TEXT("LockOnTargetCameraComponent"));
	mDialogueTargetComponent = CreateDefaultSubobject<UDialogueTargetComponent>(TEXT("DialogueTargetComponent"));

	mTeamID = ETeamType::NPC;
	mInteractableViewName = TEXT("Interact");

	AIControllerClass = ANPCAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FClassFinder<UGA_ChangeEmotion> GA_ChangeNoneEmotion(TEXT("/Script/GameplayAbilities.GameplayAbilityBlueprint'/Game/AC/Blueprint/GAS/Ability/Emotion/GA_ChangeNoneEmotion.GA_ChangeNoneEmotion_C'"));
	if (GA_ChangeNoneEmotion.Succeeded() == true)
	{
		mEmotionGAs.Add(GA_ChangeNoneEmotion.Class);
	}
}

void ANPC::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	mDialogueTargetComponent->OnPreInteract.AddDynamic(this, &ANPC::TurnToDialogueTarget);
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(mASC))
	{
		for (int32 i = 0; i < mEmotionGAs.Num(); ++i)
		{
			mASC->GiveAbility(FGameplayAbilitySpec(mEmotionGAs[i], 1, i));
		}
	}

	// 공유 뷰를 통해 Dialogue 위젯 및 ViewModel을 DialogueComponent에 자동 등록
	auto* SharedViewSubsystem = GetGameInstance()->GetSubsystem<USharedViewSubsystem>();
	if (IsValid(SharedViewSubsystem) == true)
	{
		auto* Widget = SharedViewSubsystem->GetSharedWidget(mInteractableViewName);
		auto* Handler = Cast<IInteractableViewHandler>(SharedViewSubsystem->GetSharedViewModel(mInteractableViewName));
		if (IsValid(Widget) == true && Handler != nullptr)
		{
			mDialogueTargetComponent->RegisterView(Handler, Widget, mDisplayName);
		}
	}

	auto* GameEventSubsystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
	if (IsValid(GameEventSubsystem) == false)
	{
		return;
	}
	InitByEventTrigger(GameEventSubsystem);
}

void ANPC::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// DialogueComponent 내부 참조 해제
	mDialogueTargetComponent->UnregisterView();

	Super::EndPlay(EndPlayReason);
}

UBehaviorTree* ANPC::GetBehaviorTree() const
{
	return mBehaviorTree;
}

UBlackboardData* ANPC::GetBlackBoardData() const
{
	return mBlackboardData;
}

void ANPC::SetDisplayName(const FText& DisplayName)
{
	Super::SetDisplayName(DisplayName);
	mDialogueTargetComponent->UpdateDisplayText(DisplayName);
}

void ANPC::TurnToDialogueTarget(AActor* DialogueInstigator)
{
	auto* OwnerController = GetController<AAIController>();
	if (IsValid(OwnerController) == true)
	{
		OwnerController->GetBlackboardComponent()->SetValueAsObject(TEXT("Listener"), DialogueInstigator);
	}
}
