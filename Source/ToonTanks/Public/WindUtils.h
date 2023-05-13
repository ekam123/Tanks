// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WindUtils.generated.h"

//enum Wind
//{
//	North,
//	NorthEast,
//	NorthWest,
//	West,
//	East,
//	SouthEast,
//	SouthWest,
//	South,
//	CrazyUp,
//	CrazyDown,
//	Still
//};

/**
 * 
 */
UCLASS()
class TOONTANKS_API UWindUtils: public UClass
{
	GENERATED_BODY()

private:
	float StartDelay = 3.f;
	float WindTimerDelay = 10.f;
	float MinWind = 0.05;
	float MaxWind = 0.5f;

	//	UPROPERTY(EditAnywhere, Category="Wind")
		FVector AirFlow = FVector(0, 0, 0); //
//	UPROPERTY(EditAnywhere, Category = "Wind")
		FVector CurrentWindDirection = FVector(0, 0, 0);
		FVector AirFlowScaled = FVector(0, 0, 0); 

public:
	UWindUtils();
	UWindUtils(FVector WindStrength, FVector WindDirection);
	~UWindUtils();

	//FString ReturnWindDirectionString(Wind WindFlow);
	//Wind ReturnWindDirection(FVector WindDirection);
//private:
	void GenerateWindStrength(FVector &Wind);
	void GenerateWindDirection(FVector &WindDirection);
	void CalculateWindOffset(FVector &Offset, FVector Direction,float DeltaTime);

};
