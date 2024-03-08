// LeaveMeAlone game by Netology. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "LMAReloadFinishedAnimNotify.generated.h"

class USkeletalMeshComponent;
class UAnimSequenceBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifyReloadFinishedSignature, USkeletalMeshComponent *);

/**
 * 
 */
UCLASS()
class LEAVEMEALONE_API ULMAReloadFinishedAnimNotify : public UAnimNotify
{
	GENERATED_BODY()	

public:
	
	FOnNotifyReloadFinishedSignature OnNotifyReloadFinished;

	virtual void Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation) override;
	
};
