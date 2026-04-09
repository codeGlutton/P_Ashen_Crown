#include "SightTargetSensingComponent.h"
#include "Engine/OverlapResult.h"

USightTargetSensingComponent::USightTargetSensingComponent() :
	mTraceChannel(ECollisionChannel::ECC_Camera),
	mBlockTraceChannel(ECollisionChannel::ECC_Visibility),
	mTraceTag(NAME_None),
	mTeamFilter(),
	mQuerySphereRadius(500.f),
	mQueryHalfDegree(45.f),
	mQueryForwardOffset(0.f),
	mTargetActor(nullptr),
	mCandidateActors(),
	bUpdateCandidateActors(true),
	bUpdateTargetActor(true),
	mNextCandidateActors()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.2f;

	mTeamFilter.bDetectEnemies = true;
}

void USightTargetSensingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bUpdateCandidateActors == true)
	{
		// 시야 탐지
		mNextCandidateActors.Reset();
		SensingCadidataes(mNextCandidateActors);

		// 새로운 액터 탐색
		for (AActor* Actor : mNextCandidateActors)
		{
			if (mCandidateActors.Contains(Actor) == false)
			{
				EnterCandidateActor(Actor);
			}
		}
		// 사라진 액터 탐색
		for (AActor* Actor : mCandidateActors)
		{
			if (mNextCandidateActors.Contains(Actor) == false)
			{
				LeaveCandidateActor(Actor);
			}
		}
		Swap(mNextCandidateActors, mCandidateActors);
	}

	// 타겟 액터 선택
	if (bUpdateTargetActor == true)
	{
		AActor* PreTargetActor = mTargetActor;
		mTargetActor = SelectTargetFromCandidates(mCandidateActors);
		if (PreTargetActor != mTargetActor)
		{
			OnChangeTargetActor.Broadcast(PreTargetActor, mTargetActor);
		}
	}
}

void USightTargetSensingComponent::SetTraceChannel(ECollisionChannel TraceChannel)
{
	mTraceChannel = TraceChannel;
}

ECollisionChannel USightTargetSensingComponent::GetTraceChannel() const
{
	return mTraceChannel;
}

void USightTargetSensingComponent::SetBlockTraceChannel(ECollisionChannel BlockTraceChannel)
{
	mBlockTraceChannel = BlockTraceChannel;
}

ECollisionChannel USightTargetSensingComponent::SetBlockTraceChannel() const
{
	return mBlockTraceChannel;
}

void USightTargetSensingComponent::SetTraceTag(const FName& TraceTag)
{
	mTraceTag = TraceTag;
}

const FName& USightTargetSensingComponent::GetTraceTag() const
{
	return mTraceTag;
}

void USightTargetSensingComponent::SetTeamFilter(FAISenseAffiliationFilter TeamFilter)
{
	mTeamFilter = TeamFilter;
}

FAISenseAffiliationFilter USightTargetSensingComponent::GetTeamFilter() const
{
	return mTeamFilter;
}

void USightTargetSensingComponent::SetQuerySphereRadius(float QuerySphereRadius)
{
	mQuerySphereRadius = QuerySphereRadius;
}

float USightTargetSensingComponent::GetQuerySphereRadius() const
{
	return mQuerySphereRadius;
}

void USightTargetSensingComponent::SetQueryHalfDegree(float QueryHalfDegree)
{
	mQueryHalfDegree = QueryHalfDegree;
}

float USightTargetSensingComponent::GetQueryHalfDegree() const
{
	return mQueryHalfDegree;
}

void USightTargetSensingComponent::SetQueryForwardOffset(float QueryForwardOffset)
{
	mQueryForwardOffset = QueryForwardOffset;
}

float USightTargetSensingComponent::GetQueryForwardOffset() const
{
	return mQueryForwardOffset;
}

AActor* USightTargetSensingComponent::GetTargetActor() const
{
	return mTargetActor;
}

void USightTargetSensingComponent::SensingCadidataes(TSet<TObjectPtr<AActor>>& OutCandidateActors)
{
	/* 시야 범위 내에 유효한 액터 확인 (유효성, 트레이스 채널, 태그, 팀, 탐지 범위 체킹) */

	FVector EyeLocation;
	FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector EyeForwardNormalVector = EyeRotation.Vector();
	EyeLocation += EyeForwardNormalVector * mQueryForwardOffset;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(mTraceTag, false, GetOwner());
	GetWorld()->OverlapMultiByChannel(OverlapResults, EyeLocation, EyeRotation.Quaternion(), mTraceChannel, FCollisionShape::MakeSphere(mQuerySphereRadius), Params);
	for (auto& OverlapResult : OverlapResults)
	{
		auto* OverlapActor = OverlapResult.GetActor();
		if (IsValid(OverlapActor) == true &&
			OutCandidateActors.Contains(OverlapActor) == false &&
			FAISenseAffiliationFilter::ShouldSenseTeam(GetOwner<IGenericTeamAgentInterface>(), *OverlapActor, mTeamFilter.GetAsFlags()) == true)
		{
			FVector ToTargetNormalVector = (OverlapActor->GetActorLocation() - EyeLocation).GetSafeNormal();
			const float TargetDot = FVector::DotProduct(EyeForwardNormalVector, ToTargetNormalVector);
			const float TargetDegree = FMath::RadiansToDegrees(FMath::Acos(TargetDot));

			if (mQueryHalfDegree >= TargetDegree)
			{
				FHitResult HitResult;
				FCollisionQueryParams BlockParams;
				BlockParams.AddIgnoredActor(GetOwner());
				BlockParams.AddIgnoredActor(OverlapActor);
				bool bIsBlocking = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, OverlapActor->GetActorLocation(), mBlockTraceChannel);
				if (bIsBlocking == false && IsActorValidCandidate(OverlapActor) == true)
				{
					OutCandidateActors.Add(OverlapResult.GetActor());
				}
			}
		}
	}
}
