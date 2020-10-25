// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimnotifyState.h"

#include "DrawDebugHelpers.h"

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

        AActor* IgnoreActor = Cast<AActor>(MeshComp->GetAttachParent()->GetAttachmentRootActor());
        if (IgnoreActor)
        {
            Params.AddIgnoredActor(IgnoreActor);
        }
        
        DrawDebugCapsule(MeshComp->GetWorld(), EndPos, 50.f, 15.f, MeshComp->GetSocketQuaternion("SocketTrail_End"), FColor::Black, false,5.f);
        
        bool bHit = MeshComp->GetWorld()->SweepSingleByChannel(Result, StartPos, EndPos, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeCapsule(15.f,50.f), Params);
    
        if(bHit && Result.GetActor())
        {
            DrawDebugCapsule(MeshComp->GetWorld(), Result.ImpactPoint, 50.f, 15.f, MeshComp->GetSocketQuaternion("SocketTrail_End"), FColor::Purple, false,5.f);
        }
    
    }
        
}
