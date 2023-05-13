// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Vehicle.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ATower::ATower()
{
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy(); 
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	//Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

bool ATower::DistanceToTank()
{

	Tank = Cast<AVehicle>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Tank)
	{
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		if (Distance <= FireRange)
		{
			return true;
		}
	}
	return false; 
}

void ATower::CheckFireCondition()
{
	if (Tank == nullptr)
	{
		return;
	}
	if (DistanceToTank() && Tank->bAlive && !Tank->IsInTransition) //
	{
		Fire(1);
	}
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

		if (DistanceToTank())
		{
			//Fire Turret
			//FRotator Rotation = FRotator(0.f, Tank->GetActorLocation().Rotation().Yaw, 0.f);
			RotateTurret(Tank->GetActorLocation());
		}
}


