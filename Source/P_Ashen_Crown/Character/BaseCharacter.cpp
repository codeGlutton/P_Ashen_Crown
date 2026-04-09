// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	mASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	mStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (mASC)
	{
		mASC->InitAbilityActorInfo(this, this);
	}

	if (mStimuliSourceComponent)
	{
		mStimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
	}
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return mASC;
}

void ABaseCharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	mTeamID = (ETeamType)(uint8)TeamID;
}

void ABaseCharacter::SetGenericTeamId(ETeamType TeamID)
{
	mTeamID = TeamID;
}

FGenericTeamId ABaseCharacter::GetGenericTeamId() const
{
	return (uint8)mTeamID;
}

void ABaseCharacter::SetDisplayName(const FText& DisplayName)
{
	mDisplayName = DisplayName;
}

const FText& ABaseCharacter::GetDisplayName() const
{
	return mDisplayName;
}
