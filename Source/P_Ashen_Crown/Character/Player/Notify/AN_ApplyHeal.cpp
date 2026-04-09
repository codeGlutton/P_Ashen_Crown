// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_ApplyHeal.h"
#include "../../../GAS/GameplayEffect/GE_Heal.h"

void UAN_ApplyHeal::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!MeshComp)
		return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
		return;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
	if (!ASC)
		return;

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(Owner);

	FGameplayEffectSpecHandle HealSpec = ASC->MakeOutgoingSpec(UGE_Heal::StaticClass(), 1.0f, Context);
	if (HealSpec.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToSelf(*HealSpec.Data.Get());
	}
}
