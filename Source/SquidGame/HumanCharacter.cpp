// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "StopLight.h"

// Sets default values
AHumanCharacter::AHumanCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHumanCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	StopLightRef = Cast<AStopLight>(UGameplayStatics::GetActorOfClass(GetWorld(), AStopLight::StaticClass()));

	ensure(RollMontage);

	if (StopLightRef)
	{
		StopLightRef->OnLightChange.AddDynamic(this, &AHumanCharacter::LightChanged);
	}
}

// Called every frame
void AHumanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAlive && StopLightRef && GetVelocity().Size() > 0.f && StopLightRef->CanKillPlayers())
	{
		KillPlayer();
		bIsAlive = false;
	}

	if (bIsRolling)
	{
		FVector LaunchVelocity = GetActorForwardVector() * 800.f;
		LaunchCharacter(LaunchVelocity, true, false);
	}
}

// Called to bind functionality to input
void AHumanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHumanCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHumanCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AHumanCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AHumanCharacter::LookUp);

	PlayerInputComponent->BindAction("Roll", EInputEvent::IE_Pressed, this, &AHumanCharacter::Roll);
}

void AHumanCharacter::MoveForward(float AxisValue)
{
	if (CanMove())
	{
		FVector MoveDirection = UKismetMathLibrary::GetForwardVector(GetControlRotation());
		AddMovementInput(MoveDirection, AxisValue, false);
	}
}

void AHumanCharacter::MoveRight(float AxisValue)
{
	if (CanMove())
	{
		FVector MoveDirection = UKismetMathLibrary::GetRightVector(GetControlRotation());
		AddMovementInput(MoveDirection, AxisValue, false);
	}
}

void AHumanCharacter::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AHumanCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AHumanCharacter::Roll()
{
	if (CanMove() && RollMontage)
	{
		DisableInput(nullptr);
		bIsRolling = true;
		float RollDuration = PlayAnimMontage(RollMontage);
		RollDuration *= 0.6f;
		GetWorldTimerManager().SetTimer(RollTimerHandle, this, &AHumanCharacter::StopRoll, RollDuration, false);
	}
}

void AHumanCharacter::StopRoll()
{
	bIsRolling = false;
	EnableInput(nullptr);
}

void AHumanCharacter::LightChanged(bool bIsGreenLight)
{

}

bool AHumanCharacter::CanMove_Implementation()
{
	return bIsAlive;
}