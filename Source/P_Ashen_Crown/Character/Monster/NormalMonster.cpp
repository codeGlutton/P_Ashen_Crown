// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalMonster.h"
#include "AIController.h"
#include "../../GAS/BaseAttributeSet.h"
#include "../../GAS/AttributeSet/MonsterAttributeSet.h"
#include "MonsterAnimInstance.h"
#include "../Player/PlayerCharacter.h"
//#include <P_Ashen_Crown/Character/Player/PlayerCharacter.h>

ANormalMonster::ANormalMonster()
{
	// 메시 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/AC/SourceArt/SkeletalMesh/Griffon/SKM_Griffon.SKM_Griffon'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMeshAsset(MeshAsset.Object);

	// 애님인스턴스 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		GriffonAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/AC/Blueprint/Anim/ABP_NormalMonster_Griffon.ABP_NormalMonster_Griffon_C'"));

	if (GriffonAnim.Succeeded())
		GetMesh()->SetAnimInstanceClass(GriffonAnim.Class);

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BehaviorTree(TEXT("/Script/AIModule.BehaviorTree'/Game/AC/Blueprint/AI/BT_NormalMonster.BT_NormalMonster'"));

	if (BehaviorTree.Succeeded())
		mBehaviorTree = BehaviorTree.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData>
		Blackboard(TEXT("/Script/AIModule.BlackboardData'/Game/AC/Blueprint/AI/BB_NormalMonster.BB_NormalMonster'"));

	if (Blackboard.Succeeded())
		mBlackboardData = Blackboard.Object;

	GetCapsuleComponent()->SetCapsuleHalfHeight(88);
	GetCapsuleComponent()->SetCapsuleRadius(32);

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));


	float ScaleValue = 0.6f;
	FVector Scale = FVector(ScaleValue, ScaleValue, ScaleValue);
	GetMesh()->SetWorldScale3D(FVector(Scale));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	// 시야 설정
	mSightRadius = 1200.f;
	mLoseSightRadius = 1500.f;
	mPeripheralVisionAngle = 90.f;

	mDataKey = TEXT("Griffon");

	mDisplayName = FText::FromString(TEXT("Griffon"));
}

void ANormalMonster::NormalAttack()
{
	Super::NormalAttack();

	TArray<FHitResult>	HitArray = {};

	FCollisionQueryParams	param = {};
	param.AddIgnoredActor(this);

	FVector SweepStart = GetActorLocation();
	FVector SweepEnd = SweepStart + GetActorForwardVector() * mMonsterAttribute->GetAttackDistance();

	//UE_LOG(LogTemp, Warning, TEXT("ANrmalMonster : %0.2f"), mMonsterAttribute->GetAttackDistance());

	// Trace 채널로 임시 충돌체 생성
	bool Collision = GetWorld()->SweepMultiByChannel
	(
		HitArray,
		SweepStart,
		SweepEnd,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(50.f),
		param
	);

	//DrawDebugSphere(GetWorld(), SweepStart, 25.f, 20, FColor::Red, true);
	//DrawDebugSphere(GetWorld(), SweepEnd,   25.f, 20, FColor::Green, true);
	//DrawDebugLine(GetWorld(), SweepStart, SweepEnd, FColor::Blue, true);

	if (Collision)
	{
		for (const FHitResult& Hit : HitArray)
		{
			AActor* Target = Hit.GetActor();
			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Target))
			{
				//UC_Util::Print("Normal Attack Sweep to Player Character succeeded", FColor::Red, 10.f);
				//UGameplayStatics::ApplyDamage(Target, 10.f, GetController(), this, UDamageType::StaticClass());

				// 어빌리티를 실행시키기 위한 이벤트 데이터를 만든다.
				FGameplayEventData	EventData;

				// 타겟 액터를 지정한다.
				EventData.Target = Target;

				// 타겟을 지정하기 위한 액터 지정.
				EventData.Instigator = this;

				// 이벤트를 동작시키기 위한 태그를 지정한다.
				// Ability 클래스에서 등록한 태그를 반드시 똑같이 지정해야 한다.
				EventData.EventTag = FGameplayTag::RequestGameplayTag(TEXT("Monster.Ability.Attack"));

				// HitResult 정보를 어빌리티에 전달한다.
				// 여기에서 동적할당을 했지만 어빌리티가 내부적으로 TSharedPtr을 이용해서
				// 관리하고 있기 때문에 메모리는 알아서 제거가 된다.
				FGameplayAbilityTargetData_SingleTargetHit* TargetData =
					new FGameplayAbilityTargetData_SingleTargetHit(Hit);

				EventData.TargetData.Add(TargetData);

				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this,
					EventData.EventTag, EventData);

				break;
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("HitArray"));
	}

	GetWorldTimerManager().SetTimer(mReturnCheckHandle, this, &ANormalMonster::CheckReturnToOriginalLocation, 5.0f, false);
}

void ANormalMonster::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(mMonsterAnim))
		return;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Griffon BeginPlay"));

	mOriginalLocation = GetActorLocation();
}

void ANormalMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHitReacting) 
		return;

	// 현재 액터의 월드 속도 벡터를 받아온다.
	FVector Velocity = GetVelocity();
	// 수직인 Z값을 0으로 고정. 방향(Yaw)만 사용한다.
	Velocity.Z = 0.f;

	// 속도가 0이라면 회전 계산 하지 않는다.
	// IsNearlyZero()는 0에 가까운지 확인해 주는 함수
	if (!Velocity.IsNearlyZero())
	{
		// 속도 벡터가 가리키는 방향을 구한다.
		// ToOrientationRotator()는 주어진 방향 벡터를 Pitch,Yaw,Roll 로테이터로 변환해주는 함수
		FRotator TargetRotation = Velocity.ToOrientationRotator();

		// 현재 회전에서 목표 회전으로 부드럽게 보간(보간 속도 5.f) 한다.
		const float InterpSpeed = 5.f;
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, InterpSpeed));
	}
	
}

void ANormalMonster::OnHitReactionEnd(AActor* Attacker)
{
	Super::OnHitReactionEnd(Attacker);

	if (!IsValid(Attacker))
		return;

	UE_LOG(LogTemp, Warning, TEXT("OriginalLocation = %s"), *mOriginalLocation.ToString());

	// 몬스터 위치
	FVector SelfLoc = GetActorLocation();
	// 방향 (플레이어 - 몬스터)
	FVector Dir = (Attacker->GetActorLocation() - SelfLoc).GetSafeNormal();
	// 공격자 방향으로 400cm 앞쪽의 목표 위치 계산
	FVector TargetLoc = SelfLoc + Dir * mHitMoveDistance;
	TargetLoc.Z = SelfLoc.Z;


	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		//AIController->MoveTo(TargetLoc);

		/*if (UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent())
		{
			Blackboard->SetValueAsVector(TEXT("HitMoveLocation"), TargetLoc);
			Blackboard->SetValueAsBool(TEXT("WasHit"), true);
		}*/

		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalLocation(TargetLoc);
		MoveRequest.SetAcceptanceRadius(5.0f);

		FNavPathSharedPtr NavPath;
		AIController->MoveTo(MoveRequest, &NavPath);

		UE_LOG(LogTemp, Warning, TEXT("MoveTo 실행: TargetLoc = %s"), *TargetLoc.ToString());

		GetWorldTimerManager().SetTimer(mReturnCheckHandle, this, &ANormalMonster::CheckReturnToOriginalLocation, 3.0f, false);
	}

	//UE_LOG(LogTemp, Warning, TEXT("HitMoveLocation: %s"), *TargetLoc.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Dir: %s"), *Dir.ToString());
}

void ANormalMonster::CheckReturnToOriginalLocation()
{
	if (!bHasDetectedTarget)
	{
		if (AAIController* AIController = Cast<AAIController>(GetController()))
		{
			AIController->MoveToLocation(mOriginalLocation, 5.f);
			UE_LOG(LogTemp, Warning, TEXT("타겟 없음. 원래 위치로 복귀"));
		}
	}
}

void ANormalMonster::ClearReturnTimer()
{
	GetWorldTimerManager().ClearTimer(mReturnCheckHandle);
}