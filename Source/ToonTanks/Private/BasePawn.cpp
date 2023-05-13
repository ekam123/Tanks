// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "CannonMissile.h"
#include "RocketMissile.h"
#include "Particles/ParticleSystem.h"
#include "Tank.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp); 
	//BaseMesh->SetCanEverAffectNavigation(false);
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh); 

}


// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasePawn::HandleDestruction()
{
	if (Destroyed)
	{
		
		UGameplayStatics::SpawnEmitterAtLocation(this, Destroyed, GetActorLocation(), GetActorRotation());
	}
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	}
	if (DeathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}

}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	//UE_LOG(LogTemp, Warning, TEXT("IS CHILD OF: %d"), UGameplayStatics::GetPlayerController(this, 0)->GetPawn()->GetClass()->IsChildOf(ATank::StaticClass()));
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	FString PawnName = UGameplayStatics::GetPlayerController(this, 0)->GetPawn()->GetActorNameOrLabel();
	//UE_LOG(LogTemp, Display, TEXT("[Testing] Turrent Mesh belongs to: %s %s"), *TurretMesh->GetOwner()->GetActorNameOrLabel(), *PawnName);
	TurretMesh->SetWorldRotation(FMath::RInterpTo(TurretMesh->GetComponentRotation(), LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 5.f));
}

void ABasePawn::GetTurretPitch(float Value)
{

	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Pitch = Value * UGameplayStatics::GetWorldDeltaSeconds(this)  * 10;
	UE_LOG(LogTemp, Display, TEXT("[TESTING] Current change is: %f and Value is: %f and value is: %f, %d"), DeltaRotation.Pitch, Value, TurretMesh->GetComponentRotation().Pitch, TurretMesh->GetComponentRotation().Pitch==MaxPitch);
	UE_LOG(LogTemp, Display, TEXT("[TESTING] BOOL TEST: %d %d %d %.25f"), TurretMesh->GetComponentRotation().Pitch > 10.000000, TurretMesh->GetComponentRotation().Pitch ==10.0, TurretMesh->GetComponentRotation().Pitch < 10.0, TurretMesh->GetComponentRotation().Pitch);
	if (DeltaRotation.Pitch != 0 && floor(TurretMesh->GetComponentRotation().Pitch) <= MaxPitch && floor(TurretMesh->GetComponentRotation().Pitch) >= MinPitch)
	{
		TurretMesh->AddLocalRotation(DeltaRotation);
		TurretPitch = TurretMesh->GetComponentRotation().Pitch;
		UE_LOG(LogTemp, Display, TEXT("[TESTING] Turret Pitch is: %f"), TurretPitch);

		//TurretMesh->SetWorldRotation(DeltaRotation); //Why does this not work properly?

		if (TurretMesh->GetComponentRotation().Pitch > MaxPitch)
		{
			
			float ValueToAdjust = -(TurretMesh->GetComponentRotation().Pitch - MaxPitch);
			UE_LOG(LogTemp, Display, TEXT("[TESTING] Value to adjust: %f"), ValueToAdjust);
			TurretMesh->AddLocalRotation(FRotator(ValueToAdjust, 0.0f, 0.0f));
		}
		if (TurretMesh->GetComponentRotation().Pitch < MinPitch)
		{
			float ValueToAdjust = -(TurretMesh->GetComponentRotation().Pitch - MinPitch);
			UE_LOG(LogTemp, Display, TEXT("[TESTING] Value to adjust: %f"), ValueToAdjust);
			TurretMesh->AddLocalRotation(FRotator(ValueToAdjust, 0.0f, 0.0f));
		}
		UE_LOG(LogTemp, Display, TEXT("[TESTING] Turret Pitch2 is: %f"), TurretMesh->GetComponentRotation().Pitch);
	}



}

void ABasePawn::RotateTankTurret(float Value)
{
	//FRotator DeltaRotation = FRotator::ZeroRotator;
	//DeltaRotation.Yaw = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * 60;
	//TurretMesh->AddLocalRotation(DeltaRotation);
	FHitResult HitResult;
	UGameplayStatics::GetPlayerController(this, 0)->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, false, HitResult);
	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, -1);
	float TankPitch = TurretMesh->GetComponentRotation().Pitch;//GetActorRotation().Pitch;
	float TankRoll = TurretMesh->GetComponentRotation().Roll;
	UE_LOG(LogTemp, Display, TEXT("[Testing] Turrent Rotation Pitch: %f Roll: %f"), TankPitch, TankRoll);
	FVector ToTarget = HitResult.ImpactPoint - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(TankPitch, ToTarget.Rotation().Yaw, TankRoll);
	FString PawnName = UGameplayStatics::GetPlayerController(this, 0)->GetPawn()->GetActorNameOrLabel();
	//UE_LOG(LogTemp, Display, TEXT("[BasePawn RotateTankTurret] Turrent Mesh belongs to: %s %s and movement is: %s and distance is: %s"), *TurretMesh->GetOwner()->GetActorNameOrLabel(), *PawnName, *LookAtRotation.ToString(), *ToTarget.ToString());
	if (UKismetMathLibrary::Abs(ToTarget.X) || UKismetMathLibrary::Abs(ToTarget.Y) >= 100)
	{
		TurretMesh->SetWorldRotation(FMath::RInterpTo(TurretMesh->GetComponentRotation(), LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 20.f));
	}
	

}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

TSubclassOf<class AProjectile> ABasePawn::ReturnProjectile()
{
	return CracklingClass;
}

void ABasePawn::Fire(int ProjectileType)
{
	UE_LOG(LogTemp, Display, TEXT("[PROJECTILE] Projectile Type inside BasePawn Fire Method is: %d, Range is %f"), ProjectileType, Range);
	//DrawDebugSphere(GetWorld(), ProjectileSpawnPoint->GetComponentLocation(), 20, 50, FColor::Red, false, 3);
	if (ProjectileType == 1)
	{
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		Projectile->SetOwner(this);
		Projectile->UpdateProjectileSpeed(Range); 
	} else if (ProjectileType == 2)
	{
		ACannonMissile* ProjectileTwo = GetWorld()->SpawnActor<ACannonMissile>(CannonClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		ProjectileTwo->SetOwner(this);
	}
	else if (ProjectileType == 3) 
	{
		ARocketMissile* RocketMissile = GetWorld()->SpawnActor<ARocketMissile>(RocketClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		RocketMissile->SetOwner(this); 
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("[PROJECTILE] Nothing to fire here: %d"), ProjectileType);
	}

}


