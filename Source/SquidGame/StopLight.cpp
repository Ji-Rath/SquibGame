// Fill out your copyright notice in the Description page of Project Settings.


#include "StopLight.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AStopLight::AStopLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStopLight::BeginPlay()
{
	Super::BeginPlay();

	bReplicates = true;
	bAlwaysRelevant = true;
}

// Called every frame
void AStopLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStopLight::InvertLightState()
{
	bIsGreenLight = !bIsGreenLight;

	OnLightChange.Broadcast(bIsGreenLight);

	float LightDuration = UKismetMathLibrary::RandomFloatInRange(MinLightTime, MaxLightTime);
	GetWorldTimerManager().SetTimer(LightTimerHandle, this, &AStopLight::InvertLightState, LightDuration);

	if (bIsGreenLight)
	{
		SetCanKill(false);
	}
	else
	{
		FTimerDelegate BufferDelegate;
		BufferDelegate.BindUFunction(this, FName("SetCanKill"), !bIsGreenLight);
		GetWorldTimerManager().SetTimer(LightBufferHandle, BufferDelegate, RedLightBuffer, false);
	}
}

bool AStopLight::GetIsGreenLight()
{
	return bIsGreenLight;
}

bool AStopLight::CanKillPlayers()
{
	return bCanKill;
}

void AStopLight::SetCanKill(bool bKillPlayers)
{
	bCanKill = bKillPlayers;
}

void AStopLight::BeginLightTimer()
{
	float LightDuration = UKismetMathLibrary::RandomFloatInRange(MinLightTime, MaxLightTime);
	GetWorldTimerManager().SetTimer(LightTimerHandle, this, &AStopLight::InvertLightState, LightDuration);
}

void AStopLight::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStopLight, bIsGreenLight);
	DOREPLIFETIME(AStopLight, bCanKill);
}