// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCueNotify_Effect.h"

UGameplayCueNotify_Effect::UGameplayCueNotify_Effect()
{
	GameplayCueTag = AC_PlayerState::GameplayCue_Player_Attack_BP_NormalAttackCue.GetTag();

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> AttackAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/SlashFX/Niagara/Basic/NS_Hit_Slash.NS_Hit_Slash'"));
	if (AttackAsset.Succeeded())
		mNiagara = AttackAsset.Object;
}

bool UGameplayCueNotify_Effect::OnExecute_Implementation(AActor* Target,
	const FGameplayCueParameters& Parameters) const
{
	// if (IsValid(mParticle))
	// {
	// 	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mParticle,
	// 		Target->GetActorLocation());
	// }

	if (IsValid(mNiagara))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), mNiagara,
			Target->GetActorLocation());
	}

	if (IsValid(mSound))
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), mSound,
			Target->GetActorLocation());
	}

	return Super::OnExecute_Implementation(Target, Parameters);
}
