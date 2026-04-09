// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_Skill01Start.h"
#include "../../../Character/Player/Skill/SwordProjectile.h"

UAN_Skill01Start::UAN_Skill01Start()
{
	static ConstructorHelpers::FClassFinder<ASwordProjectile> ActorClass(TEXT("/Script/Engine.Blueprint'/Game/AC/Blueprint/Actor/BPC_NyxSkillActor.BPC_NyxSkillActor_C'"));

	if (ActorClass.Succeeded())
		mProjectileClass = ActorClass.Class;
}

void UAN_Skill01Start::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Avatar = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!Avatar)
		return;
	
	FVector SpawnLocation = Avatar->GetActorLocation() + Avatar->GetActorForwardVector() * 100.f;
	FRotator SpawnRotation = Avatar->GetActorRotation();

	FActorSpawnParameters Params;
	Params.Owner = Avatar;
	Params.Instigator = Cast<APawn>(Avatar);

	ASwordProjectile* Projectile = Avatar->GetWorld()->SpawnActor<ASwordProjectile>(mProjectileClass, SpawnLocation, SpawnRotation, Params);
	Projectile->SetInstigator(Cast<APawn>(Avatar));
	Projectile->mInstigatorActor = Avatar;
	Projectile->mInstigatorController = Avatar;
	if (Projectile)
	{
		Projectile->InitDirection(Avatar->GetActorForwardVector());
	}
}
