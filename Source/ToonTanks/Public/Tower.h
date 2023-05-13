// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()

public:

	ATower();

	void HandleDestruction();


	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly)
		float FireRange = 100;

	class AVehicle* Tank;
	bool DistanceToTank(); 
	float FireRate = 2.f;
	void CheckFireCondition();
	FTimerHandle FireRateTimerHandle;
};
