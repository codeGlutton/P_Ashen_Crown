// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCueNotify_Skill01.h"

UGameplayCueNotify_Skill01::UGameplayCueNotify_Skill01()
{
	GameplayCueTag = AC_PlayerState::GameplayCue_Player_Skill.GetTag();

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> SkillHitAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/SlashFX/Niagara/Fire/NS_Hit_Slash_Fire.NS_Hit_Slash_Fire'"));
	if (SkillHitAsset.Succeeded())
		mNiagara = SkillHitAsset.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset(TEXT("/Script/Engine.SoundWave'/Game/AC/SourceArt/Sound/Kwang/SW_Kwang_Effort_Ability_RMB_01.SW_Kwang_Effort_Ability_RMB_01'"));
	if (SoundAsset.Succeeded())
		mSound = SoundAsset.Object;
}

bool UGameplayCueNotify_Skill01::OnExecute_Implementation(AActor* Target,
	const FGameplayCueParameters& Parameters) const
{
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

	return true;
}
