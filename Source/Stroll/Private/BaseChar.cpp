// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseChar.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABaseChar::ABaseChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);

	// Position the camera slightly above the eyes.
	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f + BaseEyeHeight));
	// Allow the pawn to control camera rotation.
	CameraComponent->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void ABaseChar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up "movement" bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseChar::MoveRight);

	// Set up "look" bindings.
	PlayerInputComponent->BindAxis("Turn", this, &ABaseChar::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ABaseChar::AddControllerPitchInput);

	// Set up "action" bindings.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseChar::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABaseChar::StopJump);
}

void ABaseChar::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.

	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
	
}

void ABaseChar::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void ABaseChar::StartJump()
{
	bPressedJump = true;
}

void ABaseChar::StopJump()
{
	bPressedJump = false;
}