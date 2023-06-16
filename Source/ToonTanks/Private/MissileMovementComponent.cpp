// Fill out your copyright notice in the Description page of Project Settings.


#include "MissileMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "ToonTanksGameMode.h"


void UMissileMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	GameModeRef = Cast<AToonTanksGameMode>(GetWorld()->GetAuthGameMode());
}

void UMissileMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FVector Direction = this->GetOwner()->GetActorLocation();
	FVector Direction = this->GetOwner()->GetActorForwardVector();
	FVector WindOffset; 

	if (GameModeRef)
	{
		GameModeRef->ReturnWindOffset(WindOffset, Direction, DeltaTime);
		UE_LOG(LogTemp, Display, TEXT("[WIND] WindOffset is: %s and Direction PRojectile: %s"), *WindOffset.ToString(), *Direction.ToString()); 
		this->Velocity *= WindOffset;
		UE_LOG(LogTemp, Display, TEXT("[VELOCITY] INSIDE TICK, VELOCITY: %s"), *this->Velocity.ToString()); 
	}

	//UE_LOG(LogTemp, Display, TEXT("[WIND] The dot product is: %s     %s      %s %f %f %f %f"), *Direction.ToString(),*this->Velocity.ToString(), *WindDirection.ToString(), XOffset, YOffset, ZOffset, DeltaTime);
	//UE_LOG(LogTemp, Display, TEXT("[VELOCITY] NEW VELOCITY AND SPEED MULTIPLIER and DELTATIME IS: %s %f %f, height is: %f"), *this->Velocity.ToString(), SpeedMultiplier, DeltaTime, GetActorHeight.Z);
	//UE_LOG(LogTemp, Display, TEXT("[VELOCITY] Current Velocity is: %s %s"), *CurrentVelocity.ToString(), *this->GetOwner()->GetActorLocation().ToString());
}

void UMissileMovementComponent::UpdateVelocity(float NewVelocity)
{
	this->InitialSpeed = NewVelocity;
	/*this->MaxSpeed = Velocity;*/ 
}
