// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseChar.h"


#include "BaseCharAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseChar::ABaseChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 150.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = CameraCurrentDistance; // The camera follows at this distance behind the character	
	SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller
		
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->AttachTo(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	TraceDistance = 2000.f;
}

// Called when the game starts or when spawned
void ABaseChar::BeginPlay()
{
	Super::BeginPlay();

	CurrentStamina = MaxStamina;
}

// Called every frame
void ABaseChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TraceLine();
	
	HandleStamina(DeltaTime);

	AttackTimer += DeltaTime;

	if(bIsMouseLeftButtonDown)
	{

		if(AttackTimer > 2.f)
		{
			Attack(true);
			
			bIsMouseLeftButtonDown = false;
		}  
		UE_LOG(LogTemp, Warning, TEXT("Attack Timer %f"), AttackTimer);
	}
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
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &ABaseChar::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &ABaseChar::ZoomOut);
	
	PlayerInputComponent->BindAction("MovementSpeed", IE_Pressed, this, &ABaseChar::StartRun);
	PlayerInputComponent->BindAction("MovementSpeed", IE_Released, this, &ABaseChar::StopRun);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABaseChar::InteractPressed);
	
	PlayerInputComponent->BindAction<FHandleMouse>("Attack", IE_Pressed, this, &ABaseChar::HandleMouse, true);
	PlayerInputComponent->BindAction<FHandleMouse>("Attack", IE_Released, this, &ABaseChar::HandleMouse, false);

}

void ABaseChar::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.

	const FRotator YawOnlyRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::X), Value);
}

void ABaseChar::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void ABaseChar::ZoomIn()
{
	CameraCurrentDistance -= ZoomSteps;
	if (CameraCurrentDistance < CameraMinDistance)
	{
		CameraCurrentDistance = CameraMinDistance;
	}	
	SpringArmComponent->TargetArmLength = CameraCurrentDistance;
}

void ABaseChar::ZoomOut()
{
	CameraCurrentDistance += ZoomSteps;
	if (CameraCurrentDistance > CameraMaxDistance)
	{
		CameraCurrentDistance = CameraMaxDistance;
	}

	SpringArmComponent->TargetArmLength = CameraCurrentDistance;
}

void ABaseChar::StartJump()
{
	bPressedJump = true;
}

void ABaseChar::StopJump()
{
	bPressedJump = false;
}

void ABaseChar::StartRun()
{
	bIsSprinting = true;
}

void ABaseChar::StopRun()
{
	bIsSprinting = false;
}

void ABaseChar::InteractPressed()
{
	//todo change this code block after inventory system implemented
	if(Hit.GetActor())
	{
		if(Hit.GetActor()->ActorHasTag("Sword") && !Sword)
		{
			Hit.GetActor()->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Socket_Weapon"));
			Sword = Hit.GetActor();
			//Sword->SetActorEnableCollision(false);
			IsSwordUp = true;
		}
	}
}

void ABaseChar::TraceLine()
{
	FVector Loc;
	FRotator Rot;

	GetController()->GetPlayerViewPoint(Loc, Rot);

	FVector Start = Loc;
	FVector End = Start + (Rot.Vector() * TraceDistance);

	FCollisionQueryParams TraceParams;

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	//DrawDebugPoint(GetWorld(), Start, 2.f,FColor::Yellow, false, 2.f);

	/*
	if(bHit && Hit.GetActor())
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5,5), FColor::Green, false, 2.f);

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Hit.GetActor()->GetName());
	}
	else
	{
		GEngine->ClearOnScreenDebugMessages();
	}
	*/
	
}

void ABaseChar::Attack(const bool IsHeavyAttack)
{
	const float time = GetWorld()->GetFirstPlayerController()->GetInputKeyTimeDown(FKey("LeftMouseButton"));
	
	
	UBaseCharAnimInstance* BaseCharAnimInstance = Cast<UBaseCharAnimInstance>(GetMesh()->GetAnimInstance());

	if(BaseCharAnimInstance && IsSwordUp)
	{
		if(IsHeavyAttack)
		{
			BaseCharAnimInstance->HeavyAttack();
		}
		else
		{
			BaseCharAnimInstance->Attack();
		}
	}
}

void ABaseChar::ShakeCamera()
{
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(AttackCameraShake, 1.0f);
}

void ABaseChar::SetTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1); //Global slow motion.
	this->CustomTimeDilation = 0.1; // Individual slow motion.
	
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0); //Global slow motion.
        this->CustomTimeDilation = 1.0; // Individual slow motion.
	}, .025f, false);

}

void ABaseChar::HandleStamina(const float DeltaTime)
{
	if(bIsSprinting)
	{
		CurrentStamina = FMath::Clamp(CurrentStamina - (1.f * DeltaTime), 0.f, MaxStamina);
		if(CurrentStamina > 0)
		{
			GetCharacterMovement()->MaxWalkSpeed = 600.f;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 150.f;
		}
		Timer = .0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 150.f;
		
		Timer += DeltaTime;
		if(Timer > 2.0f)
		{
			CurrentStamina = FMath::Clamp((CurrentStamina + 0.025f), 0.f, MaxStamina);
		}
	}

	OnStaminaChanged(CurrentStamina);
}

void ABaseChar::UpdateHealth(const float HealthChange)
{
	Health = FMath::Clamp(Health + HealthChange, 0.f, 100.f );
}

void ABaseChar::HandleMouse(const bool IsDown)
{
	bIsMouseLeftButtonDown = IsDown;

	if(IsDown)
	{
		AttackTimer = 0.f;
	}
	else
	{
		Attack(AttackTimer > .5f);		
	}
	
}