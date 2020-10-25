// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseCharAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STROLL_API UBaseCharAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimationUpdate")
	UAnimMontage* MeleeAttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimationUpdate")
	UAnimMontage* MeleeHeavyAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimationUpdate")
	UAnimMontage* TakeDamageMontage;
	
	UFUNCTION(BlueprintCallable, Category = "AnimationUpdate")
	void UpdateMovementSpeed();

	/*True if the character can perform a second attack*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bAcceptSecondAttackInput;

	/*Ture if the character can perform a third attack*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bAcceptsThirdAttackInput;

public:
	/*Determines which attack animation will be played*/
	void Attack();

	void HeavyAttack();
	void PlayTakeDamageAnim();
};
