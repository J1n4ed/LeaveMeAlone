// LeaveMeAlone game by Netology. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ULMAHealthComponent;
class UAnimMontage;
class ULMAWeaponComponent;
// class UCharacterMovementComponent;

#define STAMINA_MIN 20
#define STAMINA_TICK 0.1

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALMADefaultCharacter();	

	UFUNCTION()
    ULMAHealthComponent *GetHealthComponent() const 
	{
      return HealthComponent;
    }

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Stamina")
    float MaxStamina = 100.0f;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Stamina")
    float Stamina = 100.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent *SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent *CameraComponent;

    // UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    // UCharacterMovementComponent *MovementComponent;

	UPROPERTY()
    UDecalComponent *CurrentCursor = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
    UMaterialInterface *CursorMaterial = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
    FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
    ULMAHealthComponent *HealthComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage *DeathMontage;	    

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    ULMAWeaponComponent *WeaponComponent;   

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Stamina")
    float WalkSpeed = 300.0f;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Stamina")
    float SprintSpeed = 500.0f;  

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	

private:

	float YRotation = -75.0f;
    float ArmLength = 1400.0f;
    float FOV = 55.0f;
    float MaxArmLength = 2200.0f;
    float MinArmLength = 500.0f;
    const float ZoomStep = 100.0f;
    bool IsRunning = false;
    bool MoveForwardState = false;

	void MoveForward(float Value);
    void MoveRight(float Value);    
	void CameraZoom(float Value);
	void OnDeath();
	void RotationPlayerOnCursor();
	void OnHealthChanged(float NewHealth);
    void StartSprint();
    void StopSprint();
    void StaminaControl();
    bool IsStaminaFull();

};
