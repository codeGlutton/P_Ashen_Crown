// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordProjectile.h"
#include "../../../GAS/BaseAttributeSet.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASwordProjectile::ASwordProjectile()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    
    mTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    
    SetRootComponent(mTriggerBox);
    mTriggerBox->SetCollisionProfileName(TEXT("PlayerAttack"));
    mTriggerBox->OnComponentHit.AddDynamic(this, &ASwordProjectile::OnProjectileHit);
    mTriggerBox->SetBoxExtent(FVector(150.0, 200.0, 20.0));
    
    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/SlashFX/Niagara/Fire/NS_Slash_Projectile_Horizontal_Fire.NS_Slash_Projectile_Horizontal_Fire'"));
    if (NiagaraAsset.Succeeded())
    {
        mSlashEffectSystem = NiagaraAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HitNiagaraAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/SlashFX/Niagara/Fire/NS_Hit_Slash_Fire.NS_Hit_Slash_Fire'"));
    if (HitNiagaraAsset.Succeeded())
    {
        mHitNiagara = HitNiagaraAsset.Object;
    }
    
    static ConstructorHelpers::FClassFinder<UGameplayEffect> DamageEffect (TEXT("/Script/Engine.Blueprint'/Game/AC/Blueprint/GAS/Effect/BP_NyxSkill.BP_NyxSkill_C'"));
    if (DamageEffect.Succeeded())
    {
        mDamageEffectClass = DamageEffect.Class;
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset(TEXT("/Script/Engine.SoundWave'/Game/AC/SourceArt/Sound/Kwang/Custom/Skill_Hit.Skill_Hit'"));
    if (SoundAsset.Succeeded())
    {
        mHitSound = SoundAsset.Object;
    }
    
    mProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    mProjectileMovement->InitialSpeed = 1200.f;
    mProjectileMovement->MaxSpeed = 1200.f;
    mProjectileMovement->ProjectileGravityScale = 0.f;
    mProjectileMovement->bRotationFollowsVelocity = true;
    mProjectileMovement->bShouldBounce = false;

    mNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SlashEffect"));
    mNiagaraComponent->SetupAttachment(mTriggerBox);
    mNiagaraComponent->SetAutoActivate(true);

}

void ASwordProjectile::BeginPlay()
{
    Super::BeginPlay();
    // InitDirection(GetActorForwardVector());

    FTimerHandle TimerHandle;
    if (mNiagaraComponent)
    {
        mNiagaraComponent->SetAutoDestroy(true);
    }

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASwordProjectile::DeactivateEffect, 1.f, false);
    // SetLifeSpan(1.f);
}

void ASwordProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!mProjectileMovement && !mMoveDirection.IsNearlyZero())
    {
        FVector NewLocation = GetActorLocation() + mMoveDirection * mMoveSpeed * DeltaTime;
        SetActorLocation(NewLocation);
    }
}

void ASwordProjectile::InitDirection(const FVector& Direction)
{
    if (mProjectileMovement)
    {
        mProjectileMovement->Velocity = Direction * mProjectileMovement->InitialSpeed;
    }
    else
    {
        mMoveDirection = Direction.GetSafeNormal();
    }

    if (mSlashEffectSystem)
    {
        mNiagaraComponent->SetAsset(mSlashEffectSystem);
        mNiagaraComponent->Activate(true);
    }
}

void ASwordProjectile::DeactivateEffect()
{
    mTriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    mNiagaraComponent->Deactivate();
}
 
void ASwordProjectile::OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    FVector NormalImpulse, const FHitResult& Hit)
{
    mTriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    if (!mDamageEffectClass)
        return;

    UAbilitySystemComponent* TargetASC = OtherComp->GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
    UAbilitySystemComponent* SourceASC = HitComp->GetOwner()->GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
    
    if (TargetASC && OtherComp->GetCollisionProfileName() == FName(TEXT("Monster")) || OtherComp->GetCollisionProfileName() == FName(TEXT("MonsterHitMesh")))
    {
        UE_LOG(LogTemp, Warning, TEXT("Projectile hit: %s"), *OtherActor->GetName());
        
        FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
        EffectContext.AddInstigator(mInstigatorActor, mInstigatorController);
        EffectContext.AddSourceObject(this);
        EffectContext.AddHitResult(Hit);

        FGameplayEffectSpecHandle Spec = TargetASC->MakeOutgoingSpec(mDamageEffectClass, 1.0f, EffectContext);

        float Attack = SourceASC->GetSet<UBaseAttributeSet>()->GetAttackPower() * 1.5f;
        float Defense = TargetASC->GetSet<UBaseAttributeSet>()->GetDefense();

        float Dmg = Attack - Defense;

        Dmg = FMath::Max(1.f, Dmg);
		
        Spec.Data->SetSetByCallerMagnitude(AC_PlayerState::Data_Skill01_Damage.GetTag(), -Dmg);
        if (Spec.IsValid())
        {
            TargetASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
            
            if (mHitNiagara)
            {
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                    GetWorld(),
                    mHitNiagara,
                    Hit.ImpactPoint,
                    Hit.ImpactNormal.Rotation()
                );
            }

            if (mHitSound)
            {
                UGameplayStatics::SpawnSoundAtLocation(
                    GetWorld(),
                    mHitSound,
                    Hit.ImpactPoint
                );
            }
        }

    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No ASC found on actor: %s"), *OtherActor->GetName());
    }
    mNiagaraComponent->Deactivate();
}

