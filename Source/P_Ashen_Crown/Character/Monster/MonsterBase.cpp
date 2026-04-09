// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"
#include "../../GAS/BaseAttributeSet.h"
#include "../../GAS/AttributeSet/MonsterAttributeSet.h"
#include "MonsterAIController.h"
#include "MonsterAnimInstance.h"
#include "../Monster/MonsterData.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "../../GAS/GameplayAbility/GameplayAbility_Attack.h"

#include "View/MVVMView.h"
#include "P_Ashen_Crown/Widget/Main/MainViewModel.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "P_Ashen_Crown/Component/LockOnTargetActorComponent.h"

// Sets default values
AMonsterBase::AMonsterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AMonsterAIController::StaticClass();

	mMonsterAttribute = CreateDefaultSubobject<UMonsterAttributeSet>(TEXT("MonsterAttributeSet"));
	mASC->AddAttributeSetSubobject<UMonsterAttributeSet>(mMonsterAttribute);

	bUseControllerRotationYaw = false;

	// 캡슐 컴포넌트의 충돌 프로파일을 "Monster"로 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	// 캡슐 컴포넌트가 내비게이션에 영향을 주지 않도록 설정
	GetCapsuleComponent()->SetCanEverAffectNavigation(false);
	// 팀 ID를 몬스터 팀으로 설정
	SetGenericTeamId(ETeamType::Monster);

	/* 락온 기능 및 UI 설정 */

	mLockOnWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidgetComponent"));
	mLockOnWidgetComponent->SetupAttachment(GetRootComponent());
	mLockOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	mLockOnWidgetComponent->SetDrawSize(FVector2D(50., 50.));
	mLockOnWidgetComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

	mLockOnTargetActorComponent = CreateDefaultSubobject<ULockOnTargetActorComponent>(TEXT("LockOnTargetActorComponent"));
	mLockOnViewModelName = TEXT("LockOnViewModel");

	/* HP 위젯 설정 */

	mHPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidgetComponent"));
	mHPWidgetComponent->SetupAttachment(GetRootComponent());
	mHPWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	mHPWidgetComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	mHPWidgetComponent->SetDrawSize(FVector2D(500., 65.));
	mHPViewModelName = TEXT("MonsterHPBarViewModel");

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MI_Align(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/AC/SourceArt/Material/MI_Widget3DPassThroughAlign_OneSide.MI_Widget3DPassThroughAlign_OneSide'"));
	if (MI_Align.Succeeded() == true)
	{
		mHPWidgetComponent->SetMaterial(0, MI_Align.Object);
	}
}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	// 메시의 애니메이션 인스턴스를 UMonsterAnimInstance로 캐스팅하여 저장
	if(UMonsterAnimInstance* MonsterAnim = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance()))
		mMonsterAnim = MonsterAnim;

	// State.Death 태그에 변화 발생시 호출할 함수 지정
	FGameplayTag	Deathtag = FGameplayTag::RequestGameplayTag(TEXT("Monster.State.Death"));
	mASC->RegisterGameplayTagEvent(Deathtag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AMonsterBase::OnDeath);	
	
	const UMonsterData* MonsterData =
		GetDefault<UMonsterData>();

	if (MonsterData)
	{
		FMonsterTableInfo* TableInfo =
			MonsterData->FindMonsterInfo(mDataKey);

		if (TableInfo && mMonsterAttribute)
		{
			mMonsterAttribute->SetAttackPower(TableInfo->Attack);
			mMonsterAttribute->SetDefense(TableInfo->Defense);
			mMonsterAttribute->SetHP(TableInfo->HP);
			mMonsterAttribute->SetMaxHP(TableInfo->MaxHP);
			mMonsterAttribute->SetMoveSpeed(TableInfo->MoveSpeed);
			mMonsterAttribute->SetAttackDistance(TableInfo->AttackDistance);

			UE_LOG(LogTemp, Warning, TEXT("mDataKey: %s"), *mDataKey.ToString());
			UE_LOG(LogTemp, Warning, TEXT("Monster Info Attack : %f, Defense : %f, HP : %f, MoveSpeed : %f, AttackDistance : %f"),
				TableInfo->Attack, TableInfo->Defense, TableInfo->HP, 
				TableInfo->MoveSpeed, TableInfo->AttackDistance);
		}

		if (!TableInfo)
		{
			UE_LOG(LogTemp, Warning, TEXT("MonsterData Table is NULL"));
			return;
		}
	}

	mASC->GiveAbility(FGameplayAbilitySpec(UGameplayAbility_Attack::StaticClass(),
		1, 0));

	if (IsValid(mMonsterAnim))
	{
		mMonsterAnim->OnHitEnd.BindUObject(this, &AMonsterBase::OnHitReactionEnd);
	}

	/* 락온 UI 등록 */

	UUserWidget* LockOnWidget = mLockOnWidgetComponent->GetWidget();
	if (IsValid(LockOnWidget) == true)
	{
		auto* LockOnView = Cast<UMVVMView>(LockOnWidget->GetExtension(UMVVMView::StaticClass()));
		if (IsValid(LockOnView) == true)
		{
			auto* LockOnVM = NewObject<ULockOnViewModel>(this);
			LockOnView->SetViewModel(mLockOnViewModelName, LockOnVM);

			auto* LockOnViewHandler = Cast<ILockOnViewHandler>(LockOnVM);
			mLockOnTargetActorComponent->RegisterView(LockOnViewHandler, LockOnWidget);
		}
	}

	/* HP UI 등록 */

	UUserWidget* HPBarWidget = mHPWidgetComponent->GetWidget();
	if (IsValid(HPBarWidget) == true)
	{
		auto* HPBarView = Cast<UMVVMView>(HPBarWidget->GetExtension(UMVVMView::StaticClass()));
		if (IsValid(HPBarView) == true)
		{
			auto* HPBarVM = NewObject<UMonsterHPBarViewModel>(this);
			HPBarView->SetViewModel(mHPViewModelName, HPBarVM);
			HPBarVM->Initialize();
		}
	}
}

void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMonsterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

AActor* AMonsterBase::GetTargetActor() const
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!IsValid(AIController)) 
		return nullptr;

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!IsValid(Blackboard))
		return nullptr;

	return Cast<AActor>(Blackboard->GetValueAsObject(MonsterAIKey::TargetActor));
}

void AMonsterBase::ChangeAnim(EMonsterAnim Anim)
{
	float	MoveSpeed = mMonsterAttribute->GetMoveSpeed();

	/*if (Anim == EMonsterAnim::Walk)
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed * 0.5f;
	}

	else if (Anim == EMonsterAnim::Run)
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	}*/

	mMonsterAnim->ChangeAnim(Anim);
}

void AMonsterBase::NormalAttack()
{
	PlayAttackMontageSection();
}

void AMonsterBase::PlayAttackMontageSection()
{
	if (!IsValid(mMonsterAnim))
		return;

	UAnimMontage* CurrentMontage = mMonsterAnim->GetCurrentActiveMontage();

	if (!IsValid(CurrentMontage)) 
		return;

	FName CurrentSection = mMonsterAnim->Montage_GetCurrentSection();

	/*if (CurrentSection == "Attack1")
	{
		mMonsterAnim->Montage_JumpToSection("Attack1", CurrentMontage);
		return;
	}*/

	FName NextSection;

	if (CurrentSection == "Attack1")      
		NextSection = "Attack2";
	else if (CurrentSection == "Attack2") 
		NextSection = "Attack3";
	else 
		return;

	mMonsterAnim->Montage_JumpToSection(NextSection, CurrentMontage);
}

void AMonsterBase::OnDamageTaken(AActor* Attacker,const FHitResult* HitResult)
{
	if (!bIsDead && IsValid(mMonsterAnim))
	{
		bIsHitReacting = true;
		// 피격 방향 애니메이션 처리
		mMonsterAnim->SetHitDirection(GetHitDirection(this, Attacker));
		mMonsterAnim->PlayHit(Attacker);
	}

	// 회전 또는 이동 처리
	ApplyHitReaction(Attacker, HitResult);
}

void AMonsterBase::ApplyHitReaction(AActor* Attacker, const FHitResult* HitResult)
{
	if (!IsValid(Attacker))
		return;

	// 기본 회전
	FVector Direction;
	if (HitResult && HitResult->IsValidBlockingHit())
		Direction = HitResult->ImpactNormal;
	else
		Direction = Attacker->GetActorLocation() - GetActorLocation();

	Direction.Z = 0.f;
	Direction.Normalize();

	FRotator LookRot = Direction.Rotation();

	SetActorRotation(FRotator(0.f, LookRot.Yaw, 0.f));
}

void AMonsterBase::OnDeath(const FGameplayTag Tag, int32 TagCount)
{
	if (TagCount == 0)
	{
		return;
	}

	if (bIsDead)
		return;

	bIsDead = true;

	// AI 중지
	AAIController* AIController = Cast<AAIController>(GetController());
	if (IsValid(AIController))
	{
		AIController->StopMovement();

		UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
		if (IsValid(Blackboard))
		{
			Blackboard->SetValueAsBool(TEXT("IsDead"), true);
			Blackboard->ClearValue(TEXT("TargetActor"));
		}

		// 현재 실행 중인 BehaviorTree 중지!
		AIController->BrainComponent->StopLogic("Dead");
	}

	// 충돌 비활성화
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 애니메이션 중지
	if (mMonsterAnim && mMonsterAnim->IsAnyMontagePlaying())
	{
		mMonsterAnim->StopAllMontages(0.25f);
	}

	// 사망 애니메이션 재생
	HandleDeathAnimation();
}

void AMonsterBase::HandleDeathAnimation()
{
	UAnimMontage* DeathMontage = mMonsterAnim->GetMontageByType(EMonsterAnim::Death);
	if (IsValid(DeathMontage))
	{
		mMonsterAnim->Montage_Play(DeathMontage);

		// 일정 시간 뒤 Destroy
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMonsterBase::DestroyMonster, 2.0f, false);
	}
	else
	{
		RagDoll();
	}
}

void AMonsterBase::OnHitReactionEnd(AActor* Attacker)
{
	// 유효하지 않는 공격자(플레이어)일 경우 return
	if (!IsValid(Attacker))
		return;

	bIsHitReacting = false;

	// 상대 위치
	FVector AttackerLocation = Attacker->GetActorLocation();
	// 몬스터 위치
	FVector SelfLocation = GetActorLocation();

	// Z축 제거
	AttackerLocation.Z = SelfLocation.Z;

	FRotator LookRot = (AttackerLocation - SelfLocation).Rotation();

	FRotator TargetRot(0.f, LookRot.Yaw, 0.f);
	SetActorRotation(TargetRot);

	UE_LOG(LogTemp, Warning, TEXT("Rotated to attacker: %s"), *TargetRot.ToCompactString());
}

void AMonsterBase::DestroyMonster()
{
	Destroy();
}

void AMonsterBase::RagDoll()
{
	// 메시 비활성화 또는 Ragdoll
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 일정 시간 후 제거
	SetLifeSpan(5.0f);

	//Destroy();
}
