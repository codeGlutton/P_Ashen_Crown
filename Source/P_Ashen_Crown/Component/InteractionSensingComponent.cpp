#include "InteractionSensingComponent.h"
#include "P_Ashen_Crown/Component/InteractableTargetComponent.h"

UInteractionSensingComponent::UInteractionSensingComponent() :
	mTargetComponent(nullptr)
{
	bWantsInitializeComponent = true;
}

void UInteractionSensingComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OnChangeTargetActor.AddDynamic(this, &UInteractionSensingComponent::ChangeTargetComponent);
}

UInteractableTargetComponent* UInteractionSensingComponent::GetTargetComponent() const
{
	return mTargetComponent;
}

bool UInteractionSensingComponent::IsActorValidCandidate(AActor* DetectedActor) const
{
	auto* TargetComp = DetectedActor->GetComponentByClass<UInteractableTargetComponent>();
	return IsValid(TargetComp);
}

AActor* UInteractionSensingComponent::SelectTargetFromCandidates(const TSet<TObjectPtr<AActor>>& CandidateActors) const
{
	/* 가장 근접한 상호작용 액터 선별 */

	TObjectPtr<AActor> NextInteractTarget = nullptr;
	double MinDist = DBL_MAX;
	for (auto& InteractTarget : CandidateActors)
	{
		double CurDist = FVector::DistSquared(GetOwner()->GetActorLocation(), InteractTarget->GetOwner()->GetActorLocation());
		if (MinDist > CurDist)
		{
			NextInteractTarget = InteractTarget;
			MinDist = CurDist;
		}
	}
	return NextInteractTarget;
}

void UInteractionSensingComponent::ChangeTargetComponent_Implementation(AActor* PreTargetActor, AActor* NextTargetActor)
{
	/* 현재 상호작용 액터 변동 시 */

	auto* PreTargetComp = PreTargetActor == nullptr ? nullptr : PreTargetActor->GetComponentByClass<UInteractableTargetComponent>();
	auto* NextTargetComp = NextTargetActor == nullptr ? nullptr : NextTargetActor->GetComponentByClass<UInteractableTargetComponent>();
	if (IsValid(PreTargetComp) == true)
	{
		PreTargetComp->CloseInteractView();
	}
	if (IsValid(NextTargetComp) == true)
	{
		NextTargetComp->OpenInteractView(GetOwner());
	}
	mTargetComponent = NextTargetComp;
}

