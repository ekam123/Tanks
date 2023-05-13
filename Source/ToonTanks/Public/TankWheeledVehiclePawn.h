// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "TankWheeledVehiclePawn.generated.h"

/**
 * 
 */
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TOONTANKS_API ATankWheeledVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:

	ATankWheeledVehiclePawn(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	//UPROPERTY(VisibleAnywhere, Category = "Components")
	//	USpringArmComponent* SpringArmComp;
	//UPROPERTY(VisibleAnywhere, Category = "Components")
	//	UCameraComponent* CameraComp;

private:
	void MoveForward(float Value);
	void Turn(float Value);
	
};
