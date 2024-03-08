// LeaveMeAlone game by Netology. All rights reserved.


#include "Weapon/LMAWeaponComponent.h"
#include "Weapon/LMABaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"

// Sets default values for this component's properties
ULMAWeaponComponent::ULMAWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();	
    InitAnimNotify();

    Weapon->FOnOutOfAmmo.AddUObject(this, &ULMAWeaponComponent::Reload);
}


// Called every frame
void ULMAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);	
}

void ULMAWeaponComponent::SpawnWeapon()
{
    Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);

    if (Weapon) 
    {
        const auto Character = Cast<ACharacter>(GetOwner());

        if (Character) 
        {
            FAttachmentTransformRules AttachmentRules(
            EAttachmentRule::SnapToTarget, false);
            Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "r_Weapon_Socket");
        }
    }
}

void ULMAWeaponComponent::Fire() 
{
    if (Weapon && !AnimReloading) 
    {
        Weapon->Fire();
    }
}

void ULMAWeaponComponent::InitAnimNotify() 
{
    if (!ReloadMontage)
        return;
    const auto NotifiesEvents = ReloadMontage->Notifies;
    for (auto NotifyEvent : NotifiesEvents) {
        auto ReloadFinish =
            Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
        if (ReloadFinish) {
            ReloadFinish->OnNotifyReloadFinished.AddUObject(
                this, &ULMAWeaponComponent::OnNotifyReloadFinished);
            break;
        }
    }
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent *SkeletalMesh) 
{
    const auto Character = Cast<ACharacter>(GetOwner());

    if (Character->GetMesh() == SkeletalMesh) 
    {
        AnimReloading = false;
    }
}

bool ULMAWeaponComponent::CanReload() const 
{ 
    if (!AnimReloading && !Weapon->IsCurrentClipFull())
    {
        return true;
    }
    else
    {
        return false; 
    }    
}

void ULMAWeaponComponent::Reload() 
{
    if (!CanReload())
        return;

    Weapon->ChangeClip();
    AnimReloading = true;
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    Character->PlayAnimMontage(ReloadMontage);
}

void ULMAWeaponComponent::OpenFire() 
{    
    GetWorld()->GetTimerManager().SetTimer(FiringTimerHandle, this ,&ULMAWeaponComponent::Fire, ShotDelay, true);
}

void ULMAWeaponComponent::StopFire() 
{
    GetWorld()->GetTimerManager().ClearTimer(FiringTimerHandle);
}

bool ULMAWeaponComponent::GetCurrentWeaponAmmo(FAmmoWeapon &AmmoWeapon) const 
{
    if (Weapon) 
    {
        AmmoWeapon = Weapon->GetCurrentAmmoWeapon();
        return true;
    }
    return false;
}