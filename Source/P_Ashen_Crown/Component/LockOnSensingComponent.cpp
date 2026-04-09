#include "LockOnSensingComponent.h"
#include "P_Ashen_Crown/Component/LockOnTargetComponent.h"

/************************
	FPendingLockOnInput
*************************/

void FPendingLockOnInput::SetLockOn(bool bWillBeLockOn)
{
	// 수동 락온 지정 시 락온 제거 불가
	if (mManualLockOnTarget != nullptr)
	{
		return;
	}

	// 락 온을 끄는 경우는 Offset 초기화
	if (bWillBeLockOn == false)
	{
		mTargetSwitchOffset = 0;
	}
	bIsLockOn = bWillBeLockOn;
}

bool FPendingLockOnInput::IsLockOn() const
{
	return bIsLockOn;
}

void FPendingLockOnInput::SetTargetSwitchOffset(int32 TargetSwitchOffset)
{
	// 자동 락 온이 켜있을 때만 적용
	if (mManualLockOnTarget != nullptr || bIsLockOn == false)
	{
		return;
	}
	mTargetSwitchOffset = TargetSwitchOffset;
}

int32 FPendingLockOnInput::GetTargetSwitchOffset() const
{
	return mTargetSwitchOffset;
}

void FPendingLockOnInput::SetManualLockOnTarget(AActor* ManualLockOnTarget)
{
	// 수동 락온 적용 시, 수동 해제 전까지 상시 락온 및 스위칭 불가
	if (ManualLockOnTarget != nullptr)
	{
		bIsLockOn = true;
		mTargetSwitchOffset = 0;
	}
	else
	{
		bIsLockOn = false;
	}
	mManualLockOnTarget = ManualLockOnTarget;
}

AActor* FPendingLockOnInput::GetManualLockOnTarget() const
{
	return mManualLockOnTarget;
}

bool FPendingLockOnInput::ShouldUpdateTargetCandidates(bool bWasLockOn) const
{
	if (mManualLockOnTarget != nullptr)
	{
		return false;
	}
	return (bWasLockOn == false && bIsLockOn == true) || mTargetSwitchOffset != 0;
}

/************************
  ULockOnSensingComponent
*************************/

ULockOnSensingComponent::ULockOnSensingComponent()
{
	/* 탐지를 위한 초기 설정 */

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;
	bWantsInitializeComponent = true;

	mQueryForwardOffset = -100.f;
	mQueryHalfDegree = 90.f;
	mQuerySphereRadius = 2000.f;
	mTeamFilter.bDetectEnemies = true;
	mMaxLockOnDistance = 2500.f;

	/* SightTargetSensingComponent의 틱 업데이트 함수 설정 */

	bUpdateCandidateActors = false;
	bUpdateTargetActor = true;
}

void ULockOnSensingComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OnChangeTargetActor.AddDynamic(this, &ULockOnSensingComponent::ChangeTargetComponent);
}

void ULockOnSensingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// 락온에 대한 누적 인풋 값 평가
	EvaluateLockOnUpdate();

	// 기존 동작
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 누적 인풋 값 초기화
	ResetPendingState();
}

bool ULockOnSensingComponent::IsActorValidCandidate(AActor* DetectedActor) const
{
	auto* TargetComp = DetectedActor->GetComponentByClass<ULockOnTargetComponent>();
	return IsValid(TargetComp);
}

void ULockOnSensingComponent::SensingCadidataes(TSet<TObjectPtr<AActor>>& OutCandidateActors)
{
	Super::SensingCadidataes(OutCandidateActors);
	if (mTargetActor != nullptr)
	{
		OutCandidateActors.Add(mTargetActor);
	}
}

AActor* ULockOnSensingComponent::SelectTargetFromCandidates(const TSet<TObjectPtr<AActor>>& CandidateActors) const
{
	AActor* NewTargetActor = nullptr;
	if (mPendingInput.IsLockOn() == true)
	{
		// 수동 지정 대상인 경우
		if (mPendingInput.GetManualLockOnTarget() != nullptr)
		{
			return mPendingInput.GetManualLockOnTarget();
		}

		// 락온 상태로 변경한 경우, 가장 근접한 대상 탐색
		if (bIsLockOn == false)
		{
			double MinDist = DBL_MAX;
			for (auto& CandidateActor : CandidateActors)
			{
				double CurDist = FVector::DistSquared(GetOwner()->GetActorLocation(), CandidateActor->GetOwner()->GetActorLocation());
				if (MinDist > CurDist)
				{
					NewTargetActor = CandidateActor;
					MinDist = CurDist;
				}
			}
		}
		// 이전부터 락온 상태를 유지 중인 경우, 유효성과 거리 벗어남 검증
		else if (IsValid(mTargetActor) == true)
		{
			const float Dist = FVector::Dist(GetOwner()->GetActorLocation(), mTargetActor->GetActorLocation());
			if (Dist <= mMaxLockOnDistance)
			{
				NewTargetActor = mTargetActor;
			}
		}

		// 락온 스위칭을 사용한 경우
		if (NewTargetActor != nullptr && mPendingInput.GetTargetSwitchOffset() != 0)
		{
			/* 시야 기준으로 좌측 타겟 우선 정렬 */

			FVector OwnerLocation;
			FRotator OwnerRotation;
			GetOwner()->GetActorEyesViewPoint(OwnerLocation, OwnerRotation);
			FVector OwnerRightNormalVector = FRotationMatrix(OwnerRotation).GetUnitAxis(EAxis::Y).GetSafeNormal();

			TArray<TPair<double, TObjectPtr<AActor>>> CandidateActorAngles;
			for (auto& CandidateActor : CandidateActors)
			{
				FVector TowardTargetNormalVector = (OwnerRightNormalVector - CandidateActor->GetActorLocation()).GetSafeNormal();
				CandidateActorAngles.Emplace(FVector::DotProduct(OwnerRightNormalVector, TowardTargetNormalVector), CandidateActor);
			}
			CandidateActorAngles.Sort([](const TPair<double, TObjectPtr<AActor>>& Lhs, const TPair<double, TObjectPtr<AActor>>& Rhs)
				{
					return Lhs.Key > Rhs.Key;
				});

			/* 락온 대상 스위칭 */

			int32 TargetActorIndex;
			const int32 CandidateActorsNum = CandidateActorAngles.Num();
			for (TargetActorIndex = 0; TargetActorIndex < CandidateActorsNum; ++TargetActorIndex)
			{
				if (CandidateActorAngles[TargetActorIndex].Value == mTargetActor)
				{
					break;
				}
			}
			NewTargetActor = CandidateActorAngles[(TargetActorIndex + CandidateActorsNum + mPendingInput.GetTargetSwitchOffset()) % CandidateActorsNum].Value;
		}
	}
	return NewTargetActor;
}

void ULockOnSensingComponent::ChangeTargetComponent_Implementation(AActor* PreTargetActor, AActor* NextTargetActor)
{
	/* 현재 락온 타겟 컴포넌트 액터 변동 시 */

	auto* PreTargetComp = PreTargetActor == nullptr ? nullptr : PreTargetActor->GetComponentByClass<ULockOnTargetComponent>();
	auto* NextTargetComp = NextTargetActor == nullptr ? nullptr : NextTargetActor->GetComponentByClass<ULockOnTargetComponent>();
	if (IsValid(PreTargetComp) == true)
	{
		PreTargetComp->CloseLockOnView();
	}
	if (IsValid(NextTargetComp) == true)
	{
		APawn* OwnerPawn = GetOwner<APawn>();
		APlayerController* OwnerPlayerController = nullptr;
		if (IsValid(OwnerPawn) == true)
		{
			OwnerPlayerController = OwnerPawn->GetController<APlayerController>();
		}
		NextTargetComp->OpenLockOnView(OwnerPawn, OwnerPlayerController);
	}
	mTargetComponent = NextTargetComp;

	/* 현재 락온 여부 변동 시 */

	if (PreTargetActor == nullptr)
	{
		bIsLockOn = true;
		OnToggleLockOn.Broadcast(bIsLockOn);
	}
	else if (NextTargetActor == nullptr)
	{
		bIsLockOn = false;
		OnToggleLockOn.Broadcast(bIsLockOn);
	}
}

bool ULockOnSensingComponent::IsLockOn() const
{
	return bIsLockOn;
}

ULockOnTargetComponent* ULockOnSensingComponent::GetTargetComponent() const
{
	return mTargetComponent;
}

void ULockOnSensingComponent::SwitchLockOnTarget(int32 TargetSwitchOffset)
{
	mPendingInput.SetTargetSwitchOffset(TargetSwitchOffset + mPendingInput.GetTargetSwitchOffset());
}

void ULockOnSensingComponent::ToggleLockOn()
{
	mPendingInput.SetLockOn(!mPendingInput.IsLockOn());
}

void ULockOnSensingComponent::SetManualLockOnTarget(TObjectPtr<AActor> ManualLockOnTarget)
{
	mPendingInput.SetManualLockOnTarget(ManualLockOnTarget);
}

void ULockOnSensingComponent::EvaluateLockOnUpdate()
{
	bUpdateCandidateActors = mPendingInput.ShouldUpdateTargetCandidates(bIsLockOn);
}

void ULockOnSensingComponent::ResetPendingState()
{
	mPendingInput.SetLockOn(bIsLockOn);
	mPendingInput.SetTargetSwitchOffset(0);
}
