// LeaveMeAlone game by Netology. All rights reserved.

#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"

void ULMAReloadFinishedAnimNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation) 
{
  OnNotifyReloadFinished.Broadcast(MeshComp);
  Super::Notify(MeshComp, Animation);
}