// Fill out your copyright notice in the Description page of Project Settings.


#include "WindUtils.h"

UWindUtils::UWindUtils()
{
}

UWindUtils::UWindUtils(FVector WindStrength, FVector WindDirection)
{
	UE_LOG(LogTemp, Display, TEXT("[WIND] WindUtils: Wind Strength: %s WindDirection %s"), *WindStrength.ToString(), *WindDirection.ToString());
	AirFlow = WindStrength;
	CurrentWindDirection = WindDirection; 
}

UWindUtils::~UWindUtils()
{
}

//FString UWindUtils::ReturnWindDirectionString(Wind WindFlow)
//{
//	FString Result;
//	switch (WindFlow)
//	{
//	case North:
//		Result = "North";
//		break;
//	case NorthEast:
//		Result = "NorthEast";
//		break;
//	case East:
//		Result = "East";
//		break;
//	case SouthEast:
//		Result = "SouthEast";
//		break;
//	case South:
//		Result = "South";
//		break;
//	case SouthWest:
//		Result = "SouthWest";
//		break;
//	case West:
//		Result = "West";
//		break;
//	case NorthWest:
//		Result = "NorthWest";
//		break;
//	default:
//		Result = "No Wind";
//		break;
//	}
//	return Result; 
//}

//Wind UWindUtils::ReturnWindDirection(FVector WindDirection)
//{
//	Wind WindFlow = Wind::Still;
//	if (WindDirection.X > 0 && WindDirection.Y > 0)
//	{
//		WindFlow = Wind::NorthEast;
//	}
//	else if (WindDirection.X < 0 && WindDirection.Y < 0)
//	{
//		WindFlow = Wind::SouthWest;
//	}
//	else if (WindDirection.X > 0 && WindDirection.Y < 0)
//	{
//		WindFlow = Wind::NorthWest;
//	}
//	else if (WindDirection.X < 0 && WindDirection.Y > 0)
//	{
//		WindFlow = Wind::SouthEast;
//	}
//	else if (WindDirection.X < 0 && WindDirection.Y == 0)
//	{
//		WindFlow = Wind::South;
//	}
//	else if (WindDirection.X > 0 && WindDirection.Y == 0)
//	{
//		WindFlow = Wind::North;
//	}
//	else if (WindDirection.X == 0 && WindDirection.Y > 0)
//	{
//		WindFlow = Wind::East;
//	}
//	else if (WindDirection.X == 0 && WindDirection.Y < 0)
//	{
//		WindFlow = Wind::West;
//	}
//	else
//	{
//		WindFlow = Wind::Still;
//	}
//	return WindFlow; 
//}

void UWindUtils::GenerateWindStrength(FVector &Wind)
{
	float X = FMath::RandRange(MinWind, MaxWind);
	float Y = FMath::RandRange(MinWind, MaxWind);
	AirFlow = FVector(X, Y, 0); 
	float NewValue = (((X - MinWind) * (100 - 0)) / (MaxWind - MinWind)) + 0;
	float NewValueY = (((Y - MinWind) * (100 - 0)) / (MaxWind - MinWind)) + 0;
	Wind = FVector(NewValue, NewValueY, 0);  //change to member variable
	AirFlowScaled = Wind; 
}

void UWindUtils::GenerateWindDirection(FVector &WindDirection)
{
	int RandomNumberX = FMath::RandRange(-1, 1);//rand() % 10 + (-10);
	int RandomNumberY = FMath::RandRange(-1, 1); //rand() % 10 + (-10);
	int RandomNumberZ = FMath::RandRange(-1, 1); //rand() % 10 + (-10);
	UE_LOG(LogTemp, Display, TEXT("[WIND] Inside HandleWind: %d %d %d"), RandomNumberX, RandomNumberY, RandomNumberZ)
	WindDirection = FVector(RandomNumberX, RandomNumberY, RandomNumberZ);
	CurrentWindDirection = WindDirection;
}

void UWindUtils::CalculateWindOffset(FVector& Offset, FVector Direction, float DeltaTime)
{
	float XOffset;
	float YOffset;
	float ZOffset;
	/*UE_LOG(LogTemp, Display, TEXT("[WIND] Coefficient X is: %f   y is: %f   z is: %f"), Coefficient.X);*/
	if ((Direction.X < 0 && CurrentWindDirection.X < 0) || (Direction.X > 0 && CurrentWindDirection.X > 0))
	{
		XOffset = pow(1 + AirFlow.X, DeltaTime * 10);
	}
	else
	{
		XOffset = pow(1 - AirFlow.X, DeltaTime * 10);
	}
	if ((Direction.Y < 0 && CurrentWindDirection.Y < 0) || (Direction.Y > 0 && CurrentWindDirection.Y > 0))
	{
		YOffset = pow(1 + AirFlow.Y, DeltaTime * 10);
	}
	else
	{
		YOffset = pow(1 - AirFlow.Y, DeltaTime * 10);
	}
	if ((Direction.Z < 0 && CurrentWindDirection.Z < 0) || (Direction.Z > 0 && CurrentWindDirection.Z > 0))
	{
		ZOffset = pow(1 + AirFlow.Z, DeltaTime * 10);
	}
	else
	{
		ZOffset = pow(1 - AirFlow.Z, DeltaTime * 10);
	}
	Offset = FVector(XOffset, YOffset, ZOffset);
}
