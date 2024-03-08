// LeaveMeAlone game by Netology. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMABaseWeapon.generated.h"

class USkeletalMeshComponent;
class USoundWave;
class UNiagaraSystem;

DECLARE_MULTICAST_DELEGATE(FOnOutOfAmmoSignature);

USTRUCT(BlueprintType)
struct FAmmoWeapon 
{
  GENERATED_USTRUCT_BODY()
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
  int32 Bullets;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
  int32 Clips;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
  bool Infinite;
};

UCLASS()
class LEAVEMEALONE_API ALMABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALMABaseWeapon();

	void Fire();    

	FOnOutOfAmmoSignature FOnOutOfAmmo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    USkeletalMeshComponent *WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceDistance = 800.0f;

    void Shoot();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoWeapon AmmoWeapon{30, 0, true};

	FAmmoWeapon CurrentAmmoWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    USoundWave *ShootWave;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UNiagaraSystem *TraceEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FString TraceName = "Tracer";

	void SpawnTrace(const FVector &TraceStart, const FVector &TraceEnd);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float Damage = 20;

    void MakeDamage(const FHitResult &HitResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DecrementBullets();
    bool IsCurrentClipEmpty() const;
    bool IsCurrentClipFull() const;
    void ChangeClip();    

	FAmmoWeapon GetCurrentAmmoWeapon() const;
};
