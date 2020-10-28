// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimnotifyState.h"

#include "BaseAIEnemey.h"
#include "DrawDebugHelpers.h"
#include "BaseChar.h"

void UAttackAnimnotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
    if(MeshComp)
    {
        /*/////*/
        FHitResult Result;
        FVector StartPos = MeshComp->GetSocketLocation("SocketTrail_End");
        FVector EndPos = MeshComp->GetSocketLocation("SocketTrail_Start");
    
        FCollisionQueryParams Params;
        Params.bTraceComplex = true;

        ABaseChar* BaseChar = Cast<ABaseChar>(MeshComp->GetAttachParent()->GetAttachmentRootActor());

        if (BaseChar)
        {
            Params.AddIgnoredActor(BaseChar);
        }
        
       //DrawDebugCapsule(MeshComp->GetWorld(), EndPos, 50.f, 15.f, MeshComp->GetSocketQuaternion("SocketTrail_End"), FColor::Black, false,5.f);
        
        bool bHit = MeshComp->GetWorld()->SweepSingleByChannel(Result, StartPos, EndPos, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeCapsule(15.f,50.f), Params);
    
        if(bHit && Result.GetActor())
        {
            ABaseAIEnemey* EnemyActor = Cast<ABaseAIEnemey>(Result.GetActor());
            
            if(EnemyActor)
            {
                if(bIsFirstTime)
                {
                    EnemyActor->TakeDamage(50.f);
                          
                    if(BaseChar)
                    {
                        BaseChar->ShakeCamera();
                    }
                    
                    bIsFirstTime = false;
                }
            }
           // DrawDebugCapsule(MeshComp->GetWorld(), Result.ImpactPoint, 50.f, 15.f, MeshComp->GetSocketQuaternion("SocketTrail_End"), FColor::Purple, false,5.f);
        }
    }
}

void UAttackAnimnotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
    float TotalDuration)
{
    bIsFirstTime = true;

}

void UAttackAnimnotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    bIsFirstTime = false;
}
