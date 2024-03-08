// LeaveMeAlone game by Netology. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/LMABaseWeapon.h"
#include "LMAWeaponComponent.generated.h"

class ALMABaseWeapon;
class ULMAReloadFinishedAnimNotify;
class USkeletalMeshComponent;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAVEMEALONE_API ULMAWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULMAWeaponComponent();	

	void Fire();

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ALMABaseWeapon> WeaponClass;

    UPROPERTY()
    ALMABaseWeapon *Weapon = nullptr;

	void SpawnWeapon();

	FTimerHandle FiringTimerHandle;		

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage *ReloadMontage;

	bool AnimReloading = false;

    void Reload();
    void InitAnimNotify();
    void OnNotifyReloadFinished(USkeletalMeshComponent *SkeletalMesh);
    bool CanReload() const;		

	UFUNCTION(BlueprintCallable)
	void OpenFire();
    UFUNCTION(BlueprintCallable)
    void StopFire();

	UFUNCTION(BlueprintCallable)
    bool GetCurrentWeaponAmmo(FAmmoWeapon &AmmoWeapon) const;

private:

    bool ReadyToFire = false;
    float ShotDelay = 0.1f;	
};