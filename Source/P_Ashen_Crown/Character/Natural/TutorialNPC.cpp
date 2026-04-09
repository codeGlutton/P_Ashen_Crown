#include "TutorialNPC.h"
#include "P_Ashen_Crown/Component/DialogueTargetComponent.h"
#include "P_Ashen_Crown/Singleton/Subsystem/GameEventSubsystem.h"

#include "Engine/AssetManager.h"

#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"

#include "P_Ashen_Crown/Character/Monster/NormalMonster.h"

ATutorialNPC::ATutorialNPC()
{
	PrimaryActorTick.bCanEverTick = true;

	mDisplayName = FText::FromString(TEXT("절뚝거리는 노인"));
	mSeeOldManEventName = TEXT("SeeOldMan");
	mMeetOldManEventName = TEXT("MeetOldMan");
	mHuntMonsterEventName = TEXT("HuntGriffon");
	mRewardFromOldManEventName = TEXT("RewardFromOldMan");

	GetCapsuleComponent()->SetCapsuleHalfHeight(90.f);
	GetCapsuleComponent()->SetCapsuleRadius(30.f);
	GetMesh()->SetRelativeLocation(FVector(0., 0., -90.));
	GetMesh()->SetRelativeRotation(FRotator(0., -90., 0.));

	TSoftObjectPtr<UDataTable> DialogueTable(FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/AC/Data/Dialogue/DT_OldManDialogue.DT_OldManDialogue'")));
	FName StartKey = TEXT("Meet");
	mDialogueTargetComponent->SetDialogueDT(DialogueTable);
	mDialogueTargetComponent->SetDialogueStartKey(StartKey);

	mSeeOldManSequence = FSoftObjectPath(TEXT("/Script/LevelSequence.LevelSequence'/Game/AC/Blueprint/Scene/LS_SeeOldMan.LS_SeeOldMan'"));
	mMeetOldManSequence = FSoftObjectPath(TEXT("/Script/LevelSequence.LevelSequence'/Game/AC/Blueprint/Scene/LS_MeetOldMan.LS_MeetOldMan'"));
	mTutorialMonsterClass = FSoftClassPath(TEXT("/Script/Engine.Blueprint'/Game/AC/Blueprint/Anim/BP_NormalMonster_Griffon.BP_NormalMonster_Griffon'"));

	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_OldMan(TEXT("/Script/Engine.AnimBlueprint'/Game/AC/Blueprint/Anim/NPC/ABP_OldMan.ABP_OldMan'"));
	if (ABP_OldMan.Succeeded() == true)
	{
		GetMesh()->SetAnimInstanceClass(ABP_OldMan.Class);
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_NPC(TEXT("/Script/AIModule.BehaviorTree'/Game/AC/Blueprint/AI/NPC/BT_TutorialNPC.BT_TutorialNPC'"));
	if (BT_NPC.Succeeded() == true)
	{
		mBehaviorTree = BT_NPC.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB_NPC(TEXT("/Script/AIModule.BlackboardData'/Game/AC/Blueprint/AI/NPC/BB_TutorialNPC.BB_TutorialNPC'"));
	if (BB_NPC.Succeeded() == true)
	{
		mBlackboardData = BB_NPC.Object;
	}

	static ConstructorHelpers::FClassFinder<UGA_ChangeEmotion> GA_ChangeScaredEmotion(TEXT("/Script/GameplayAbilities.GameplayAbilityBlueprint'/Game/AC/Blueprint/GAS/Ability/Emotion/GA_ChangeScaredEmotion.GA_ChangeScaredEmotion_C'"));
	if (GA_ChangeScaredEmotion.Succeeded() == true)
	{
		mEmotionGAs.Add(GA_ChangeScaredEmotion.Class);
	}

	static ConstructorHelpers::FClassFinder<UGA_ChangeEmotion> GA_ChangeDisappointedEmotion(TEXT("/Script/GameplayAbilities.GameplayAbilityBlueprint'/Game/AC/Blueprint/GAS/Ability/Emotion/GA_ChangeDisappointedEmotion.GA_ChangeDisappointedEmotion_C'"));
	if (GA_ChangeDisappointedEmotion.Succeeded() == true)
	{
		mEmotionGAs.Add(GA_ChangeDisappointedEmotion.Class);
	}

	static ConstructorHelpers::FClassFinder<UGA_ChangeEmotion> GA_ChangeTiredEmotion(TEXT("/Script/GameplayAbilities.GameplayAbilityBlueprint'/Game/AC/Blueprint/GAS/Ability/Emotion/GA_ChangeTiredEmotion.GA_ChangeTiredEmotion_C'"));
	if (GA_ChangeTiredEmotion.Succeeded() == true)
	{
		mEmotionGAs.Add(GA_ChangeTiredEmotion.Class);
	}
}

void ATutorialNPC::InitByEventTrigger(UGameEventSubsystem* GameEventSubsystem)
{
	Super::InitByEventTrigger(GameEventSubsystem);

	// 이미 퀘스트를 해결한 적이 있다면...
	if (GameEventSubsystem->WasTriggerEvent(mRewardFromOldManEventName) == true)
	{
		mDisplayName = FText::FromString(TEXT("타렌스"));
	}
	else if (GameEventSubsystem->WasTriggerEvent(mHuntMonsterEventName) == true)
	{
		mASC->TryActivateAbilitiesByTag(AC_NPCEmotion::NPC_Emotion_Scared.GetTag().GetSingleTagContainer());
	}
}

void ATutorialNPC::BeginPlay()
{
	Super::BeginPlay();

	auto* GameEventSubsystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
	if (IsValid(GameEventSubsystem) == false)
	{
		return;
	}
	GameEventSubsystem->BindToEvent(mSeeOldManEventName, FOnCallEvent::FDelegate::CreateUObject(this, &ATutorialNPC::ReceieveGameEvent_SeeOldMan));
	GameEventSubsystem->BindToEvent(mMeetOldManEventName, FOnCallEvent::FDelegate::CreateUObject(this, &ATutorialNPC::ReceieveGameEvent_MeetOldMan));
}

void ATutorialNPC::ReceieveGameEvent_SeeOldMan(AActor* EventInstigator, AActor* MainTarget)
{
	// 레벨 시퀀스 로드 후 비동기 실행
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(mSeeOldManSequence.ToSoftObjectPath(), [this, MainTarget]()
		{
			FMovieSceneSequencePlaybackSettings PlaybackSettings;
			PlaybackSettings.bDisableMovementInput = true;
			PlaybackSettings.bDisableLookAtInput = true;
			PlaybackSettings.bHideHud = true;

			ALevelSequenceActor* SequenceActor = nullptr;
			auto* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), mSeeOldManSequence.Get(), PlaybackSettings, SequenceActor);
			if (IsValid(SequenceActor) == true)
			{
				SequenceActor->AddBindingByTag(TEXT("Player"), MainTarget);
			}
			SequencePlayer->Play();
		});
}

void ATutorialNPC::ReceieveGameEvent_MeetOldMan(AActor* EventInstigator, AActor* MainTarget)
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, MainTarget]()
		{
			TArray<FSoftObjectPath> UsingAssets;
			UsingAssets.Add(mMeetOldManSequence.ToSoftObjectPath());
			UsingAssets.Add(mTutorialMonsterClass.ToSoftObjectPath());

			UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(UsingAssets, [this, MainTarget]()
				{

					FMovieSceneSequencePlaybackSettings PlaybackSettings;
					PlaybackSettings.bDisableMovementInput = true;
					PlaybackSettings.bDisableLookAtInput = true;
					PlaybackSettings.bHideHud = true;

					ALevelSequenceActor* SequenceActor = nullptr;
					auto* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), mMeetOldManSequence.Get(), PlaybackSettings, SequenceActor);
					if (IsValid(SequenceActor) == true)
					{
						FActorSpawnParameters SpawnParams;
						SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						auto* TutorialMonster = GetWorld()->SpawnActor<ANormalMonster>(mTutorialMonsterClass.Get(), SpawnParams);
						if (IsValid(TutorialMonster) == true)
						{
							SequenceActor->AddBindingByTag(TEXT("Player"), MainTarget);
							SequenceActor->AddBindingByTag(TEXT("Monster"), TutorialMonster);
							SequencePlayer->Play();
							TutorialMonster->OnEndPlay.AddDynamic(this, &ATutorialNPC::SuccessHuntMonsterQuest);
						}
					}
				});
		}), 1.5f, false);
}

void ATutorialNPC::SuccessHuntMonsterQuest(AActor* Monster, EEndPlayReason::Type EndPlayReason)
{
	auto* GameEventSubsystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
	if (IsValid(GameEventSubsystem) == false)
	{
		return;
	}
	GameEventSubsystem->CallEvent(mHuntMonsterEventName, Monster);
}
