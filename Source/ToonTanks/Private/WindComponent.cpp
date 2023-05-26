// Fill out your copyright notice in the Description page of Project Settings.


#include "WindComponent.h"

// Sets default values for this component's properties
UWindComponent::UWindComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWindComponent::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle WindTimer;
	FTimerDelegate TimerDelegateWind = FTimerDelegate::CreateUObject(this, &UWindComponent::HandleWind);
	GetWorld()->GetTimerManager().SetTimer(WindTimer, TimerDelegateWind, WindTimerDelay, true);
	
}


// Called every frame
void UWindComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWindComponent::HandleWind()
{
	UE_LOG(LogTemp, Display, TEXT("[WIND] Inside HandleWind:"))
	
	GenerateWindStrength();
	GenerateWindDirection();
	FString DirectionString = ReturnWindDirectionString(ReturnWindDirection(CurrentWindDirection));
	UE_LOG(LogTemp, Display, TEXT("[WIND] Airflow is: %s and Direction is: %s"), *AirFlow.ToString(), *CurrentWindDirection.ToString());

	OnAirFlowChanged.Broadcast(AirFlowScaled, CurrentWindDirection, DirectionString, CreateWindStrengthString(), WindAngle);

}

//void UWindComponent::ReturnWindOffset(FVector& WindOffset, FVector Direction, float DeltaTime)
//{
//	CalculateWindOffset(WindOffset, Direction, DeltaTime);
//}

FString UWindComponent::ReturnWindDirectionString(Wind WindFlow)
{
	FString Result;
	switch (WindFlow)
	{
	case North:
		Result = "N";
		WindAngle = 0.0f;
		break;
	case NorthEast:
		Result = "NE";
		WindAngle = 45.0f;
		break;
	case East:
		Result = "E";
		WindAngle = 90.0f;
		break;
	case SouthEast:
		Result = "SE";
		WindAngle = 135.0f;
		break;
	case South:
		Result = "S";
		WindAngle = 180.0f;
		break;
	case SouthWest:
		Result = "SW";
		WindAngle = 225.0f;
		break;
	case West:
		Result = "W";
		WindAngle = 270.0f;
		break;
	case NorthWest:
		Result = "NW";
		WindAngle = 315.0f;
		break;
	default:
		Result = "No Wind";
		WindAngle = 0.0f;
		break;
	}
	return Result;
}

Wind UWindComponent::ReturnWindDirection(FVector WindDirection)
{
	Wind WindFlow = Wind::Still;
	if (WindDirection.X > 0 && WindDirection.Y > 0)
	{
		WindFlow = Wind::NorthEast;
	}
	else if (WindDirection.X < 0 && WindDirection.Y < 0)
	{
		WindFlow = Wind::SouthWest;
	}
	else if (WindDirection.X > 0 && WindDirection.Y < 0)
	{
		WindFlow = Wind::NorthWest;
	}
	else if (WindDirection.X < 0 && WindDirection.Y > 0)
	{
		WindFlow = Wind::SouthEast;
	}
	else if (WindDirection.X < 0 && WindDirection.Y == 0)
	{
		WindFlow = Wind::South;
	}
	else if (WindDirection.X > 0 && WindDirection.Y == 0)
	{
		WindFlow = Wind::North;
	}
	else if (WindDirection.X == 0 && WindDirection.Y > 0)
	{
		WindFlow = Wind::East;
	}
	else if (WindDirection.X == 0 && WindDirection.Y < 0)
	{
		WindFlow = Wind::West;
	}
	else
	{
		WindFlow = Wind::Still;
	}
	return WindFlow;
}

void UWindComponent::GenerateWindStrength()
{
	float X = FMath::RandRange(MinWind, MaxWind);
	float Y = FMath::RandRange(MinWind, MaxWind);
	AirFlow = FVector(X, Y, 0);
	//Scale up to 0 to 100 for User
	float NewValueX = (((X - MinWind) * (100 - 0)) / (MaxWind - MinWind)) + 0;
	float NewValueY = (((Y - MinWind) * (100 - 0)) / (MaxWind - MinWind)) + 0;
	AirFlowScaled = FVector(NewValueX, NewValueY, 0);  //change to member variable
}

void UWindComponent::GenerateWindDirection()
{
	int RandomNumberX = FMath::RandRange(-1, 1);//rand() % 10 + (-10);
	int RandomNumberY = FMath::RandRange(-1, 1); //rand() % 10 + (-10);
	int RandomNumberZ = FMath::RandRange(-1, 1); //rand() % 10 + (-10);
	UE_LOG(LogTemp, Display, TEXT("[WIND] Inside HandleWind: %d %d %d"), RandomNumberX, RandomNumberY, RandomNumberZ)
	CurrentWindDirection = FVector(RandomNumberX, RandomNumberY, RandomNumberZ);
}

void UWindComponent::CalculateWindOffset(FVector& Offset, FVector Direction, float DeltaTime)
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

FString UWindComponent::CreateWindStrengthString()
{

	int X = AirFlowScaled.X;
	int Y = AirFlowScaled.Y;
	FString string = FString::Printf(TEXT("X: %d Y: %d"), X, Y);
	return string;

}

