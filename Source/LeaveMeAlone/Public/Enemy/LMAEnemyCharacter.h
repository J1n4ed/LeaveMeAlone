// LeaveMeAlone game by Netology. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMAEnemyCharacter.generated.h"

class ULMAHealthComponent;
class UBehaviorTree;
class UAnimMontage;

UCLASS()
class LEAVEMEALONE_API ALMAEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALMAEnemyCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree *BehaviorTreeAsset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    ULMAHealthComponent *HealthComponent;	

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage *DeathMontage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};