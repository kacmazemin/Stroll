// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIEnemey.h"

#include "BaseCharAnimInstance.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ABaseAIEnemey::ABaseAIEnemey()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ABaseAIEnemey::OnPawnSeen);

}

// Called when the game starts or when spawned
void ABaseAIEnemey::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseAIEnemey::OnPawnSeen(APawn* SeenPawn)
{

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("OnPawnSeen"));
}

// Called every frame
void ABaseAIEnemey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseAIEnemey::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABaseAIEnemey::TakeDamage(float DamageAmount)
{
	UBaseCharAnimInstance* BaseCharAnimInstance = Cast<UBaseCharAnimInstance>(GetMesh()->GetAnimInstance());

	if(BaseCharAnimInstance)
	{
		BaseCharAnimInstance->PlayTakeDamageAnim();
	}
	
	Health = FMath::Clamp(Health - DamageAmount, 0.f, 100.f);
	if(Health == 0.0f)
	{
		
		BaseCharAnimInstance->PlayDeathAnimMontage();

		FTimerHandle TimerHandle;

		GetWorldTimerManager().SetTimer(TimerHandle, [=]()
        {
            this->Destroy();
        }, 2.f, false);
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, FString::Printf(TEXT("Current Health => %f"), Health));
	return DamageAmount;
}