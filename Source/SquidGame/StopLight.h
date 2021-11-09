// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StopLight.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStopLightChange, bool, bIsGreenLight);

UCLASS()
class SQUIDGAME_API AStopLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStopLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	bool bIsGreenLight = true;

	UFUNCTION()
	void InvertLightState();

	UPROPERTY(EditAnywhere)
	float MaxLightTime = 15.f;

	UPROPERTY(EditAnywhere)
	float MinLightTime = 5.f;

	FTimerHandle LightTimerHandle;

	UFUNCTION(BlueprintCallable)
	bool GetIsGreenLight();

	UPROPERTY()
	FStopLightChange OnLightChange;

	FTimerHandle LightBufferHandle;
	
	UPROPERTY(Replicated)
	bool bCanKill = false;

	UFUNCTION()
	bool CanKillPlayers();

	UFUNCTION()
	void SetCanKill(bool bKillPlayers);

	UPROPERTY(EditAnywhere)
	float RedLightBuffer = 0.25f;

	UFUNCTION(BlueprintCallable)
	void BeginLightTimer();
};
