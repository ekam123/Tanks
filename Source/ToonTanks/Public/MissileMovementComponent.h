// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/MovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MissileMovementComponent.generated.h"


/**
 * 
 */
class AToonTanksGameMode;

UCLASS()
class TOONTANKS_API UMissileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void UpdateVelocity(float NewVelocity);

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	AToonTanksGameMode* GameModeRef;
};
