// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharAnimInstance.h"

void UBaseCharAnimInstance::UpdateMovementSpeed()
{
    //Get the pawn and if it's valid, update the movement speed to update
    //the movement animation
    APawn* Pawn = TryGetPawnOwner();

    if (Pawn)
    {
        MovementSpeed = Pawn->GetVelocity().Size();
    }
}

void UBaseCharAnimInstance::Attack()
{
    if(MeleeAttackMontage)
    {
        FName CurrentSection = Montage_GetCurrentSection(MeleeAttackMontage);

        if(CurrentSection.IsNone())
        {
            Montage_Play(MeleeAttackMontage);
        }
        else if(CurrentSection.IsEqual("FirstAttack") && bAcceptSecondAttackInput)
        {
            Montage_JumpToSection(FName("SecondAttack"), MeleeAttackMontage);
        }
        else if(CurrentSection.IsEqual("SecondAttack") && bAcceptsThirdAttackInput)
        {
            Montage_JumpToSection(FName("ThirdAttack"), MeleeAttackMontage);
        }
        
    }    
}
