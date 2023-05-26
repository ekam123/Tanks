// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WindComponent.generated.h"

enum Wind
{
	North,
	NorthEast,
	NorthWest,
	West,
	East,
	SouthEast,
	SouthWest,
	South,
	CrazyUp,
	CrazyDown,
	Still
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnAirFlowChangedTwo, FVector, AirStrength, FVector, AirDirection, FString, AirDirectionString, FString, AirStrengthString, float, DirectionAngle);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UWindComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWindComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float MinWind = 0.05;
	float MaxWind = 0.5f;
	float WindAngle = 0.0f; 

	UPROPERTY(BlueprintAssignable)
		FOnAirFlowChangedTwo OnAirFlowChanged;
	UPROPERTY(EditAnywhere, Category="Wind")
		float WindTimerDelay = 10.f;
	UPROPERTY(EditAnywhere, Category="Wind")
		FVector AirFlow = FVector(0, 0, 0); //
	UPROPERTY(EditAnywhere, Category = "Wind")
		FVector CurrentWindDirection = FVector(0, 0, 0);
		FVector AirFlowScaled = FVector(0, 0, 0);


public:

	//void ReturnWindOffset(FVector& WindOffset, FVector Direction, float DeltaTime);
	void HandleWind();
	void CalculateWindOffset(FVector& Offset, FVector Direction, float DeltaTime);


private:
	FString CreateWindStrengthString();
	FString ReturnWindDirectionString(Wind WindFlow);
	Wind ReturnWindDirection(FVector WindDirection);
	void GenerateWindStrength();
	void GenerateWindDirection();
		
};

