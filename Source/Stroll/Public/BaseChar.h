// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseChar.generated.h"

UCLASS()
class STROLL_API ABaseChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float TraceDistance;

	UPROPERTY(EditAnywhere)
	float ZoomSteps = 10.0f;
	
	UPROPERTY(EditAnywhere)
	float CameraMaxDistance = 650.0f;
	
	UPROPERTY(EditAnywhere)
	float CameraMinDistance = 200.0f;

	UPROPERTY(EditAnywhere)
	float CameraCurrentDistance = 350.0f;

	UPROPERTY()
	float CurrentStamina = 0.f;

	UPROPERTY()
	float MaxStamina = 5.f;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnStaminaChanged(const float value);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GamePlay")
	bool IsSwordUp = false;
	
protected:
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void ZoomIn();
	void ZoomOut();
	void StartJump();
	void StopJump();
	void StartRun();
	void StopRun();

	void InteractPressed();
	
	void TraceLine();

	void Attack();
	
private:
	float Timer = 0.f;
	
	bool bIsSprinting = false;
	
	FHitResult Hit;
	
	AActor* Sword;

	void HandleStamina(const float DeltaTime);
};
