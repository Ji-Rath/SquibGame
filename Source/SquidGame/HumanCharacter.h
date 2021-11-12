// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HumanCharacter.generated.h"

class AStopLight;

UCLASS()
class SQUIDGAME_API AHumanCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHumanCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION()
	void MoveRight(float AxisValue);

	UFUNCTION()
	void Turn(float AxisValue);

	UFUNCTION()
	void LookUp(float AxisValue);

	/* Actual roll functionality */
	UFUNCTION(NetMulticast, Reliable)
	void Roll();
	void Roll_Implementation();

	/* Called when the player wants to roll */
	UFUNCTION(Server, Reliable)
	void WantsToRoll();
	void WantsToRoll_Implementation();

	AStopLight* StopLightRef = nullptr;

	UPROPERTY()
	bool bIsRolling = false;

	UPROPERTY(EditAnywhere)
	UAnimMontage* RollMontage = nullptr;

	UFUNCTION()
	void StopRoll();

	FTimerHandle RollTimerHandle;

	UFUNCTION(BlueprintImplementableEvent)
	void KillPlayer();

	bool bIsAlive = true;

	UFUNCTION()
	void LightChanged(bool bIsGreenLight);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanMove();
	bool CanMove_Implementation();
};
