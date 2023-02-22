// Udemy Shoot Them Up


#include "Components/USTUWeaponComponent.h"
#include "Weapon/USTUBaseWeapon.h"
#include <GameFramework/Character.h>
#include "Animations/USTUEquipFinishedAnimNotify.h"
#include "Animations/USTUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

constexpr static int32 WeaponNum = 2;

UUSTUWeaponComponent::UUSTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UUSTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapon items"), WeaponNum);

	CurrenWeaponIndex = 0;
	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrenWeaponIndex);
}

void UUSTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{
	CurrentWeapon = nullptr;
	for (auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();

	Super::EndPlay(EndPlayReason);
}

void UUSTUWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;

	for (auto OneWeaponData : WeaponData)
	{
		auto Weapon = GetWorld()->SpawnActor<AUSTUBaseWeapon>(OneWeaponData.WeaponClass);
		if (!Weapon) continue;

		Weapon->OnClipEmpty.AddUObject(this, &UUSTUWeaponComponent::OnEmptyClip);
		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
}

void UUSTUWeaponComponent::AttachWeaponToSocket(AUSTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UUSTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		UE_LOG(LogWeaponComponent, Display, TEXT("Invalid weapon index"));
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
	}

	CurrentWeapon = Weapons[WeaponIndex];
	//CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
	const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {//
		return Data.WeaponClass == CurrentWeapon->GetClass(); 
		});//
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
	EquipAnimInProgress = true;
	PlayAnimMontage(EquipAnimMontage);
}

void UUSTUWeaponComponent::Fire()
{
	if (!CanFire()) return;
	CurrentWeapon->Fire();
}

void UUSTUWeaponComponent::StartFire()
{
	if (!CanFire()) return;
	CurrentWeapon->StartFire();
}

void UUSTUWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

void UUSTUWeaponComponent::NextWeapon()
{
	if (!CanEquip()) return;
	CurrenWeaponIndex = (CurrenWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrenWeaponIndex);
}

void UUSTUWeaponComponent::Reload()
{
	ChangeClip();
}

void UUSTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	Character->PlayAnimMontage(Animation);
}

void UUSTUWeaponComponent::InitAnimations()
{
	auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UUSTUEquipFinishedAnimNotify>(EquipAnimMontage);
	if (EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &UUSTUWeaponComponent::OnEquipFinished);
	}
	else
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
		checkNoEntry();
	}

	for (auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UUSTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if (!ReloadFinishedNotify) 
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
			checkNoEntry();
		}

		ReloadFinishedNotify->OnNotified.AddUObject(this, &UUSTUWeaponComponent::OnReloadFinished);
	}
}

void UUSTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;

	//UE_LOG(LogWeaponComponent, Display, TEXT("Equip finished"));
	EquipAnimInProgress = false;
}

void UUSTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;

	//UE_LOG(LogWeaponComponent, Display, TEXT("Equip finished"));
	ReloadAnimInProgress = false;
}

bool UUSTUWeaponComponent::CanFire() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;// && CurrentWeapon->CanReload();
}

bool UUSTUWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UUSTUWeaponComponent::CanReload() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

void UUSTUWeaponComponent::OnEmptyClip()
{
	ChangeClip();
}

void UUSTUWeaponComponent::ChangeClip()
{
	if (!CanReload()) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	ReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}

bool UUSTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	if (CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool UUSTUWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	if (CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}
