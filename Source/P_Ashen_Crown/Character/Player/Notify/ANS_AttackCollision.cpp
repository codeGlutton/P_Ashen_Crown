// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_AttackCollision.h"
#include "GenericTeamAgentInterface.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemGlobals.h"
#include "../../../GAS/BaseAttributeSet.h"
#include "../../../Character/BaseCharacter.h"

UANS_AttackCollision::UANS_AttackCollision()
{
	static ConstructorHelpers::FClassFinder<UGameplayEffect> GE_Damage_BP(TEXT("/Script/Engine.Blueprint'/Game/AC/Blueprint/GAS/Effect/BP_AttackDamage.BP_AttackDamage_C'"));
	if (GE_Damage_BP.Succeeded())
	{
		mDamageEffectClass = GE_Damage_BP.Class;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/SlashFX/Niagara/Basic/NS_Hit_Slash.NS_Hit_Slash'"));
	if (NiagaraAsset.Succeeded())
	{
		mHitNiagara = NiagaraAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset(TEXT("/Script/Engine.SoundWave'/Game/AC/SourceArt/Sound/Kwang/Custom/Sword_Hit.Sword_Hit'"));
	if (SoundAsset.Succeeded())
	{
		mHitSound = SoundAsset.Object;
	}

	Radius = 100.f;
	AttackSocketName = TEXT("weapon_Attackpoint");
	
}

void UANS_AttackCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	mAlreadyHitActors.Empty();
}

void UANS_AttackCollision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	PerformHitCheck(Owner, MeshComp);
}

void UANS_AttackCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	mAlreadyHitActors.Empty();
}

void UANS_AttackCollision::PerformHitCheck(AActor* Owner, USkeletalMeshComponent* MeshComp)
{
	if (!mDamageEffectClass)
		return;

	UWorld* World = Owner->GetWorld();
	if (!World)
		return;

	FVector Start = MeshComp->GetSocketLocation(AttackSocketName);
	FVector End = Start + FVector(1.f);

	DrawDebugSphere(World, Start, Radius, 16, FColor::Red, false, 0.1f);

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	bool bHit = World->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(Radius),
		Params
	);

	if (!bHit) return;

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();

		if (!HitActor || mAlreadyHitActors.Contains(HitActor))
			continue;

		const IGenericTeamAgentInterface* TargetTeam = Cast<IGenericTeamAgentInterface>(HitActor);
		if (!TargetTeam)
			continue;

		const uint8 TeamId = TargetTeam->GetGenericTeamId().GetId();
		if (static_cast<ETeamType>(TeamId) != ETeamType::Monster)
			continue;

		mAlreadyHitActors.Add(HitActor);

		UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
		UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(HitActor);

		if (!SourceASC || !TargetASC)
			continue;

		FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
		Context.AddSourceObject(Owner);

		FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(mDamageEffectClass, 1.f, Context);

		float Attack = SourceASC->GetSet<UBaseAttributeSet>()->GetAttackPower();
		float Defense = TargetASC->GetSet<UBaseAttributeSet>()->GetDefense();
		float Dmg = FMath::Max(1.f, Attack - Defense);

		Spec.Data->SetSetByCallerMagnitude(AC_PlayerState::Data_NormalAttack_Damage.GetTag(), -Dmg);
		if (Spec.IsValid())
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*Spec.Data);
		}

		if (mHitNiagara)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				World,
				mHitNiagara,
				Hit.ImpactPoint,
				Hit.ImpactNormal.Rotation()
			);
		}

		if (mHitSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(
				World,
				mHitSound,
				Hit.ImpactPoint
			);
		}
	}
}