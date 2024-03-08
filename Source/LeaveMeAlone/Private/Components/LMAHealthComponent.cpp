// LeaveMeAlone game by Netology. All rights reserved.


#include "Components/LMAHealthComponent.h"

// Sets default values for this component's properties
ULMAHealthComponent::ULMAHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void ULMAHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;	

	AActor *OwnerComponent = GetOwner();

	if (OwnerComponent)
	{
          OwnerComponent->OnTakeAnyDamage.AddDynamic(this, &ULMAHealthComponent::OnTakeAnyDamage);
	}	
}

// Called every frame
/*
void ULMAHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
*/

void ULMAHealthComponent::OnTakeAnyDamage(AActor *DamagedActor, float Damage, const class UDamageType *DamageType, class AController *InstigatedBy, AActor *DamageCauser) 
{
	if (IsDead())
	{
          return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);    

	if (IsDead())
	{
          OnDeath.Broadcast();
	}
}

bool ULMAHealthComponent::IsAline() const
{ return Health > 0.0f; }

bool ULMAHealthComponent::IsDead() const
{ 
	return Health <= KINDA_SMALL_NUMBER; 
}

bool ULMAHealthComponent::AddHealth(float NewHealth) 
{
        if (IsDead() || IsHealthFull())
			return false;
		else
		{
			Health = FMath::Clamp(Health + NewHealth, 0.0f, MaxHealth);			
			return true;
		}          
}

bool ULMAHealthComponent::IsHealthFull() const 
{
        return FMath::IsNearlyEqual(Health, MaxHealth);
}