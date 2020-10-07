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

	UPROPERTY(EditAnywhere)
	float ZoomSteps = 10.0f;
	
	UPROPERTY(EditAnywhere)
	float CameraMaxDistance = 650.0f;
	
	UPROPERTY(EditAnywhere)
	float CameraMinDistance = 200.0f;

	UPROPERTY(EditAnywhere)
	float CameraCurrentDistance = 350.0f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Handles input for moving forward and backward.
	UFUNCTION()
    void MoveForward(float Value);

	// Handles input for moving right and left.
	UFUNCTION()
    void MoveRight(float Value);

	UFUNCTION()
	void ZoomIn();
	
	UFUNCTION()
	void ZoomOut();

	// Sets jump flag when key is pressed.
	UFUNCTION()
    void StartJump();

	// Clears jump flag when key is released.
	UFUNCTION()
    void StopJump();

	// FPS camera.
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent;
};
