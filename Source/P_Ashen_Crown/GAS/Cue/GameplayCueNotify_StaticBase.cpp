// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCueNotify_StaticBase.h"

UGameplayCueNotify_StaticBase::UGameplayCueNotify_StaticBase()
{
}

bool UGameplayCueNotify_StaticBase::OnExecute_Implementation(AActor* Target,
	const FGameplayCueParameters& Parameters) const
{
	if (IsValid(mParticle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mParticle,
			Target->GetActorLocation());
	}

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

bool UGameplayCueNotify_StaticBase::OnActive_Implementation(AActor* Target,
	const FGameplayCueParameters& Parameters) const
{
	if (IsValid(mParticle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mParticle,
			Target->GetActorLocation());
	}

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

bool UGameplayCueNotify_StaticBase::WhileActive_Implementation(AActor* Target,
	const FGameplayCueParameters& Parameters) const
{
	return true;
}

bool UGameplayCueNotify_StaticBase::OnRemove_Implementation(AActor* Target,
	const FGameplayCueParameters& Parameters) const
{
	return true;
}
