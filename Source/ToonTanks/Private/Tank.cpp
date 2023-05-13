// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ToonTanksPlayerController.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "WeaponsSystemComponent.h"
#include "HealthComponent.h"

ATank::ATank()
{
	//PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp); 
	
	//UWeaponsSystemComponent WeaponsSystem = CreateDefaultSubobject<UWeaponsSystemComponent>(TEXT("WeaponsComponent"));
	
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}


void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = GetController<APlayerController>();
	WeaponTypes.Add(0);
	WeaponTypes.Add(1);
	WeaponTypes.Add(2); 

	auto ActorComponent = this->GetComponentByClass(UWeaponsSystemComponent::StaticClass());
	WeaponsSystemComponent = Cast<UWeaponsSystemComponent>(ActorComponent);
}


void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//	FHitResult HitResult;
	//	UGameplayStatics::GetPlayerController(this, 0)->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, false, HitResult);
	//	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, -1);
	//	RotateTurret(HitResult.ImpactPoint);
	//	if (TankPlayerController)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("[PLAYERCONTROLLER] PLAYER CONTROLLER PAWN: %s"), *TankPlayerController->GetPawn()->GetActorNameOrLabel())
	//}
}


void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAxis(TEXT("RaiseTurret"), this, &ATank::RaiseTurret);
	PlayerInputComponent->BindAxis(TEXT("RotateTurret"), this, &ATank::RotateTurretAlt);
	PlayerInputComponent->BindAxis(TEXT("SetTurretRange"), this, &ATank::SetTurretRange);
	PlayerInputComponent->BindAxis(TEXT("RotateWeapons"), this, &ATank::RotateThroughWeapons);
	PlayerInputComponent->BindAxis(TEXT("SelectWeapon"), this, &ATank::SelectWeapon);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::FireProjectile);
}

void ATank::FireWeaponChangeEvent()
{
	//OnWeaponChanged.Broadcast(WeaponIndex);
	WeaponsSystemComponent->SelectProjectile(WeaponIndex);
	WeaponsSystemComponent->BroadcastProjectileAmmoCounts();
	//auto TEst = this->GetComponentByClass(UHealthComponent::StaticClass());
	//UHealthComponent* HealthComponent = Cast<UHealthComponent>(TEst);
	//HealthComponent->UpdatePlayerScore(); 
	UE_LOG(LogTemp, Display, TEXT("[WEAPONINDEX] WeaponChangeEventFired: %d"), WeaponIndex);
	if (WeaponsSystemComponent)
	{
		WeaponsSystemComponent->UpdateScore(0); 
	}
}
FString ATank::ReturnPlayerName()
{

	return TankDetails.Name;
}


void ATank::MoveForward(float Value)
{
	//UE_LOG(LogTemp, Display, TEXT("Move Forward is being called!!!"));
	if (DistanceTravelled <= TotalAllowedDistance)
	{
		FVector DeltaLocation = FVector::ZeroVector;
		float Distance = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * Speed;
		DeltaLocation.X = Distance;
		FHitResult HitResult; 
		AddActorLocalOffset(DeltaLocation, false, &HitResult);
		UE_LOG(LogTemp, Display, TEXT("Reached the end of......!!!"));
		DistanceTravelled += FMath::Abs(DeltaLocation.X);
		UE_LOG(LogTemp, Display, TEXT("[HITRESULT] Total Distance Travelled: %s"), *HitResult.ToString());
		//FVector Direction = this->GetActorForwardVector().GetSafeNormal();
		//FVector CurrentLocation = this->GetActorLocation(); 
		//float Thrust = Speed * 1000; 
		//UE_LOG(LogTemp, Display, TEXT("[TANKMOVEMENTTEST] Forward Vector: %s"), *Direction.ToString());
		//BaseMesh->AddForce(Direction * Thrust);
		//FVector FinalLocation = this->GetActorLocation();
		//DistanceTravelled += FMath::Abs(FinalLocation.X - CurrentLocation.X);
		//UE_LOG(LogTemp, Display, TEXT("Total Distance Travelled: %f"), DistanceTravelled);
	} else
	{
		HasReachedDistanceLimit = true;
	}
}

void ATank::Turn(float Value)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * TurnRate;
	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::RaiseTurret(float Value)
{
	GetTurretPitch(Value);
	
}

void ATank::RotateTurretAlt(float Value)
{
//	if (TankPlayerController)
//{
//	FHitResult HitResult;
//	TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
//	Super::RotateTurret(HitResult.ImpactPoint);
//}
	RotateTankTurret(Value);
}

void ATank::SetTurretRange(float Value)
{
	
	if (Value == 1.0 && Range < 2.0)
	{
		Range += 0.01;
		if (Range > 2.0)
		{
			Range = 2.0; 
		}
	} else if (Value == -1.0 && Range > 0.0)
	{
		Range -= 0.01;
		if (Range < 0.0)
		{
			Range = 0.0;
		}
	}
	UE_LOG(LogTemp, Display, TEXT("[RANGE] %f %f %d"), Value, Range, Range > 2.0); 

}

void ATank::SelectWeapon(float Value)
{
	if (Value != 0.0 && static_cast<int>(Value) != WeaponIndex)
	{
		WeaponIndex = static_cast<int>(Value);
		WeaponsSystemComponent->SelectProjectile(Value);
		//WeaponIndex = static_cast<int>(Value);
		UE_LOG(LogTemp, Display, TEXT("[WEAPONINDEX] RotateThroughWeapons Value: %d"), WeaponIndex);
		//OnWeaponChanged.Broadcast(WeaponIndex);
		////UE_LOG(LogTemp, Display, TEXT("[PROJECTILE] RotateThroughWeapons Value: %f"), Value);
	}
}


void ATank::RotateThroughWeapons(float Value)
{
	//TODO
	if (Value != 0.0)
	{

		//UE_LOG(LogTemp, Display, TEXT("[PROJECTILE] RotateThroughWeapons Value: %f"), Value);
	}
	
}


void ATank::FireProjectile()
{
	/*ProjectileType = 2;*/
	
	if (!IsInTransition)
	{
		WeaponIndex = WeaponsSystemComponent->ReturnProjectileIndex(); 
		UE_LOG(LogTemp, Display, TEXT("[PROJECTILE] Fire method being overridden, Projectile Type is: %d"), WeaponIndex);
		Fire(WeaponIndex);
		WeaponsSystemComponent->UpdateProjectileAmmoCount(WeaponIndex); 
		IsInTransition = true; 
	}
}

void ATank::Fire(int ProjectileType)
{

	Super::Fire(ProjectileType);
	UE_LOG(LogTemp, Display, TEXT("[MOUSETEST] TESTING WHEN THIS IS CALLED!!!!"));
	//UNCOMMENT AFTERS TESTING
	//IsInTransition = true; 
	//AToonTanksPlayerController* ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	//ToonTanksPlayerController->SetPlayerEnabledState(false);
	//FTimerHandle PlayerEnableTimerHandle;
	//FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, &AToonTanksPlayerController::ChangePlayer, false);
	//GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, TimerDelegate, 2.0f, false);
}

void ATank::SetPlayerDetails(FPlayerDetails TankDetail)
{
	TankDetails = TankDetail; 
}