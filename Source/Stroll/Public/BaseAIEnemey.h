// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseAIEnemey.generated.h"

class UPawnSensingComponent;

UCLASS()
class STROLL_API ABaseAIEnemey : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAIEnemey();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, Category = "AISettings")
	bool bIsPatrolEnable = false;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditInstanceOnly, Category = "AISettings", meta = (EditCondition = "bIsPatrolEnable") )
	AActor* FirstTargetPosition;
	
	UPROPERTY(EditInstanceOnly, Category = "AISettings", meta = (EditCondition = "bIsPatrolEnable") )
	AActor* SecondTargetPosition;

	AActor* CurrentTargetPosition;
	
	UFUNCTION()
    void OnPawnSeen(APawn* SeenPawn);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float TakeDamage(float DamageAmount);

private:
	float Health = 100.f;

	void MoveToNextLocation();
};
