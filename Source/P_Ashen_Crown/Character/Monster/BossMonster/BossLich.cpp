// Fill out your copyright notice in the Description page of Project Settings.


#include "BossLich.h"
#include "../../Monster/BossSkill/BossLichProjectile.h"
#include "../../Monster/BossSkill/BossLichMagicFloor.h"
#include "AIController.h"
#include "../MonsterAnimInstance.h"
#include "../../Player/PlayerCharacter.h"
#include "../MonsterData.h"
#include "../../../GAS/GameplayEffect/GE_MagicFloorCooldown.h"
#include "../../../GAS/GameplayEffect/GE_ProjectileSkillCooldown.h"
#include "../../../GAS/GameplayEffect/GE_TeleportCooldown.h"



ABossLich::ABossLich()
{
	// 메시 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/AC/SourceArt/SkeletalMesh/Sevarog/SKM_SevarogBloodred.SKM_SevarogBloodred'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMeshAsset(MeshAsset.Object);

	// 애님인스턴스 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		GriffonAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/AC/Blueprint/Anim/ABP_BossMonster_Lich.ABP_BossMonster_Lich_C'"));

	if (GriffonAnim.Succeeded())
		GetMesh()->SetAnimInstanceClass(GriffonAnim.Class);

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BehaviorTree(TEXT("/Script/AIModule.BehaviorTree'/Game/AC/Blueprint/AI/BT_BossLich.BT_BossLich'"));

	if (BehaviorTree.Succeeded())
		mBehaviorTree = BehaviorTree.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData>
		Blackboard(TEXT("/Script/AIModule.BlackboardData'/Game/AC/Blueprint/AI/BB_BossLich.BB_BossLich'"));

	if (Blackboard.Succeeded())
		mBlackboardData = Blackboard.Object;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	//GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);

	GetCapsuleComponent()->SetCapsuleHalfHeight(140);
	GetCapsuleComponent()->SetCapsuleRadius(50);

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -140.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	// 시야 설정
	mSightRadius = 5000.f;
	mLoseSightRadius = 5500.f;
	mPeripheralVisionAngle = 90.f;

	mDataKey = TEXT("Lich");

	mDisplayName = FText::FromString(TEXT("Lich"));
}

void ABossLich::NormalAttack()
{
	TArray<FHitResult>	HitArray = {};

	FCollisionQueryParams	param = {};
	param.AddIgnoredActor(this);

	FVector SweepStart = GetActorLocation();
	FVector SweepEnd = SweepStart + GetActorForwardVector() * 300.f;

	// Trace 채널로 임시 충돌체 생성
	bool Collision = GetWorld()->SweepMultiByChannel
	(
		HitArray,
		SweepStart,
		SweepEnd,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(100.f),
		param
	);

	DrawDebugSphere(GetWorld(), SweepStart, 25.f, 20, FColor::Red, true);
	DrawDebugSphere(GetWorld(), SweepEnd, 25.f, 20, FColor::Green, true);
	DrawDebugLine(GetWorld(), SweepStart, SweepEnd, FColor::Blue, true);

	if (Collision)
	{
		for (const FHitResult& Hit : HitArray)
		{
			AActor* Target = Hit.GetActor();
			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Target))
			{
				//UGameplayStatics::ApplyDamage(Target, 10.f, GetController(), this, UDamageType::StaticClass());

				FGameplayEventData	EventData;
				EventData.Target = Target;
				EventData.Instigator = this;

				EventData.EventTag = FGameplayTag::RequestGameplayTag(TEXT("Monster.Ability.Attack"));

				FGameplayAbilityTargetData_SingleTargetHit* TargetData =
					new FGameplayAbilityTargetData_SingleTargetHit(Hit);

				EventData.TargetData.Add(TargetData);

				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this,
					EventData.EventTag, EventData);

				break;
			}
		}
	}
}

void ABossLich::UseProjectileSkill()
{

	if (!mProjectileClass)
	{
		UE_LOG(LogTemp, Error, TEXT("발사체 클래스가 설정되지 않았습니다!"));
		return;
	}

	AActor* Target = GetTargetActor();
	if (!IsValid(Target))
	{
		UE_LOG(LogTemp, Error, TEXT("TargetActor가 설정되지 않았습니다."));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("마법 시전!"));

	if (mASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.ProjectileSkill"))))
	{
		UE_LOG(LogTemp, Warning, TEXT("쿨타임 중 - 스킬 사용 불가"));
		return;
	}

	// 쿨타임 이펙트 적용
	FGameplayEffectContextHandle EffectContext = mASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle =
		mASC->MakeOutgoingSpec(UGE_ProjectileSkillCooldown::StaticClass(), 1.f, EffectContext);

	if (SpecHandle.IsValid())
	{
		// SetByCaller로 쿨타임 지속시간 5초 설정
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Monster.Data.CoolDown.Projectile")), mProjectileCooldown);

		SpecHandle.Data->DynamicGrantedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.ProjectileSkill")));

		// Effect 적용
		mASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		UpdateCooldownBlackboard();

		if (SpecHandle.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("쿨타임 이펙트 적용 성공"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("쿨타임 이펙트 적용 실패"));
		}
	}

	// 방향 계산
	FVector MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Projectile_L"));
	FVector TargetLoc = GetTargetActor()->GetActorLocation();

	if (IsValid(mSkillFireSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, mSkillFireSound, MuzzleLoc);
	}

	FVector Dir = (TargetLoc - MuzzleLoc).GetSafeNormal();
	FRotator MuzzleRot = Dir.Rotation();

	UE_LOG(LogTemp, Warning, TEXT("스킬 발사 위치: %s"), *MuzzleLoc.ToString());


	// 발사체 생성
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Owner = this;

	ABossLichProjectile* Projectile = GetWorld()->SpawnActor<ABossLichProjectile>(mProjectileClass, MuzzleLoc, MuzzleRot, Params);
	if (IsValid(Projectile))
	{
		//Projectile->SetOwnerController(GetController());
		Projectile->SetOwner(this);
		Projectile->SetInstigator(Cast<APawn>(this));
		UE_LOG(LogTemp, Warning, TEXT("BP_BossLichSkill1 발사체 생성 성공"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BP_BossLichSkill1 발사체 생성 실패"));
	}

	DrawDebugLine(GetWorld(), MuzzleLoc, MuzzleLoc + Dir * 1000, FColor::Red, false, 3.0f, 0, 2.0f);
}

void ABossLich::UseMagicFloorSkill()
{
	if (mASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.MagicFloorSkill"))))
	{
		UE_LOG(LogTemp, Warning, TEXT("쿨타임 중 - 스킬 사용 불가"));
		return;
	}

	// 쿨타임 이펙트 적용
	FGameplayEffectContextHandle EffectContext = mASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle =
		mASC->MakeOutgoingSpec(UGE_MagicFloorCooldown::StaticClass(), 1.f, EffectContext);

	if (SpecHandle.IsValid())
	{
		// SetByCaller로 쿨타임 지속시간 15초 설정
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Monster.Data.CoolDown.MagicFloor")), mMagicFloorCooldown);

		SpecHandle.Data->DynamicGrantedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.MagicFloorSkill")));

		// Effect 적용
		mASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		UpdateCooldownBlackboard();

		if (SpecHandle.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("쿨타임 이펙트 적용 성공"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("쿨타임 이펙트 적용 실패"));
		}
	}

	if (IsValid(mMagicFloorActor))
	{
		// 범위 내 타겟 데미지 처리
		mMagicFloorActor->OnDamage();
		// 재사용 방지
		//mMagicFloorActor = nullptr; 
		UE_LOG(LogTemp, Warning, TEXT("MagicFloorSkill damage applied"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MagicFloorActor is invalid"));
	}
}

void ABossLich::SpawnMagicFloorActor()
{
	// 액터 생성
	if (!IsValid(mMagicFloorActorClass))
	{
		UE_LOG(LogTemp, Error, TEXT("MagicFloorActorClass is not set"));
		return;
	}

	FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT("ik_foot_MagicFloor")) + FVector(0, 0, 10);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	if (IsValid(mFloorSkillSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, mFloorSkillSound, SpawnLocation);
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Owner = this;

	mMagicFloorActor = GetWorld()->SpawnActor<ABossLichMagicFloor>(mMagicFloorActorClass, SpawnLocation, SpawnRotation, Params);
	if (IsValid(mMagicFloorActor))
	{
		mMagicFloorActor->SetOwnerController(GetController());
		mMagicFloorActor->SetDamageRadius(400.f);
		UE_LOG(LogTemp, Warning, TEXT("장판 스킬 위치 : %s"), *SpawnLocation.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MagicFloorActor spawn failed"));
	}	
}

void ABossLich::UseTeleportSkill()
{
	// 이미 쿨타임 중이면 스킬 발동 못함
	if (mASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.TeleportSkill"))))
	{
		UE_LOG(LogTemp, Warning, TEXT("Teleport 스킬 쿨타임 중!"));
		return;
	}

	// MonsterData 가져오기
	UMonsterData* MonsterData = NewObject<UMonsterData>();
	if (!IsValid(MonsterData))
		return;

	FVector CurrentLocation = GetActorLocation();

	// 랜덤좌표 가져오기
	FBossTeleportLocation* LocationData = MonsterData->GetRandomTeleportLocation(CurrentLocation);
	if (!LocationData)
		return;

	// 순간이동 처리
	SetActorLocation(LocationData->TeleportLocation);

	UE_LOG(LogTemp, Warning, TEXT("[Teleport] Boss 순간이동: %s"), *LocationData->TeleportLocation.ToString());

	// 쿨타임 설정
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle =
		GetAbilitySystemComponent()->MakeOutgoingSpec(UGE_TeleportCooldown::StaticClass(), 1.f, EffectContext);

	if (SpecHandle.IsValid())
	{
		// 순간이동 쿨타임 시간 15초
		SpecHandle.Data->SetSetByCallerMagnitude(
			FGameplayTag::RequestGameplayTag(TEXT("Monster.Data.CoolDown.Teleport")), mTeleportCooldown);

		SpecHandle.Data->DynamicGrantedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.TeleportSkill")));

		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		UE_LOG(LogTemp, Warning, TEXT("텔레포트 쿨타임 적용 완료"));
	}
}

void ABossLich::HandleDeathAnimation()
{

	if (mMonsterAnim)
	{
		UAnimMontage* DeathMontage = mMonsterAnim->GetMontageByType(EMonsterAnim::Death);
		if (IsValid(DeathMontage))
		{
			mMonsterAnim->Montage_Play(DeathMontage);

			// 2.6초 후 Destroy
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AMonsterBase::DestroyMonster, 2.6f, false);
			OnActorDestroy.Broadcast(mMagicFloorActor);
			return;
		}
	}
}

void ABossLich::UpdateCooldownBlackboard()
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent())
		{
			bool MagicFloorCooldown = mASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.MagicFloorSkill")));
			bool TeleportCooldown = mASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.TeleportSkill")));

			Blackboard->SetValueAsBool(TEXT("MagicFloorCooldown"), MagicFloorCooldown);
			Blackboard->SetValueAsBool(TEXT("TeleportCooldown"), TeleportCooldown);

			UE_LOG(LogTemp, Warning, TEXT("Blackboard 갱신: FloorSkillCooldown = %s, TeleportCooldown = %s"),
				MagicFloorCooldown ? TEXT("true") : TEXT("false"),
				TeleportCooldown ? TEXT("true") : TEXT("false"));
		}
	}
}