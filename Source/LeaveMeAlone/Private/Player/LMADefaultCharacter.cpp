// LeaveMeAlone game by Netology. All rights reserved.


#include "Player/LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/LMAHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/LMAWeaponComponent.h"

// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArmComponent->SetupAttachment(GetRootComponent());	

	SpringArmComponent->SetUsingAbsoluteRotation(true);
    SpringArmComponent->TargetArmLength = ArmLength;
    SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;
    SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    CameraComponent->SetFieldOfView(FOV);
    CameraComponent->bUsePawnControlRotation = false;      

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");    

    WeaponComponent = CreateDefaultSubobject<ULMAWeaponComponent>("Weapon");
}

// Called when the game starts or when spawned
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    OnHealthChanged(HealthComponent->GetHealth());

    if (CursorMaterial) {
          CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(
              GetWorld(), CursorMaterial, CursorSize, FVector(0));
        }

    HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);

    // MovementComponent->MaxWalkSpeed = WalkSpeed;
    Stamina = MaxStamina;
}

// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);    

    if (!(HealthComponent->IsDead()))
    {
          RotationPlayerOnCursor();
    }    
    
    StaminaControl();
}

// Called to bind functionality to input
void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
    PlayerInputComponent->BindAxis("CameraZoom", this, &ALMADefaultCharacter::CameraZoom);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALMADefaultCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALMADefaultCharacter::StopSprint);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::OpenFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &ULMAWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Reload);
}

void ALMADefaultCharacter::MoveForward(float Value) {
    AddMovementInput(GetActorForwardVector(), Value);
    if (Value > KINDA_SMALL_NUMBER)
    {
          MoveForwardState = true;
    }
    else
    {
          MoveForwardState = false;
    }
}

void ALMADefaultCharacter::MoveRight(float Value) {
    AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter::CameraZoom(float Value) 
{
    float ZoomResult = SpringArmComponent->TargetArmLength + (Value * ZoomStep);

    if (ZoomResult <= MinArmLength)
          ZoomResult = MinArmLength;
    else if (ZoomResult >= MaxArmLength)
          ZoomResult = MaxArmLength;

    SpringArmComponent->TargetArmLength = ZoomResult;    
}

void ALMADefaultCharacter::OnDeath()
{
    CurrentCursor->DestroyRenderState_Concurrent();

    PlayAnimMontage(DeathMontage);

    GetCharacterMovement()->DisableMovement();

    SetLifeSpan(15.0f);

    if (Controller)
    {
          Controller->ChangeState(NAME_Spectating);
    }
}

void ALMADefaultCharacter::RotationPlayerOnCursor() 
{
    APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    
    if (PC) 
    {
          FHitResult ResultHit;
          PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);

          float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
          SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));

          if (CurrentCursor) 
          {
            CurrentCursor->SetWorldLocation(ResultHit.Location);
          }
    }
}

void ALMADefaultCharacter::OnHealthChanged(float NewHealth) 
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Health = %f"), NewHealth));
}

void ALMADefaultCharacter::StartSprint() 
{ 
    if (Stamina >= STAMINA_MIN && MoveForwardState)
    {
          IsRunning = true; 
          this->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }    
}

void ALMADefaultCharacter::StopSprint() 
{ 
    IsRunning = false; 
    MoveForwardState = false;
    this->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ALMADefaultCharacter::StaminaControl()
{ 
    if (IsRunning && MoveForwardState)
    {
      Stamina = FMath::Clamp(Stamina - (STAMINA_TICK * 2), 0.0f, MaxStamina);
    }
    else
    {
      Stamina = FMath::Clamp(Stamina + STAMINA_TICK, 0.0f, MaxStamina);
    }
    
    if (Stamina == KINDA_SMALL_NUMBER)
    {
        StopSprint();
    }
}

bool ALMADefaultCharacter::IsStaminaFull() 
{
    return MaxStamina - Stamina > KINDA_SMALL_NUMBER;
}