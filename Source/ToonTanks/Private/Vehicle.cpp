// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "DisplayDebugHelpers.h"
#include "ToonTanksPlayerController.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "WeaponsSystemComponent.h"
#include "HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projectile.h"
#include "CannonMissile.h"
#include "RocketMissile.h"


FName AVehicle::VehicleMovementComponentName(TEXT("VehicleMovementComp"));
FName AVehicle::VehicleMeshComponentName(TEXT("VehicleMesh"));
AVehicle::AVehicle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(VehicleMeshComponentName);
	Mesh->SetCollisionProfileName(UCollisionProfile::Vehicle_ProfileName);
	Mesh->BodyInstance.bSimulatePhysics = false;
	Mesh->BodyInstance.bNotifyRigidBodyCollision = true;
	Mesh->BodyInstance.bUseCCD = true;
	Mesh->bBlendPhysics = true;
	Mesh->SetGenerateOverlapEvents(true);
	Mesh->SetCanEverAffectNavigation(false);
	RootComponent = Mesh;

	//UMaterialInstanceDynamic* MI = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), this);
	////auto Material = Mesh->GetMaterial(0);
	//FString MaterialString; 
	//MI->GetFName().ToString(MaterialString);
	//UE_LOG(LogTemp, Display, TEXT("[MATERIALMESH] Material at index 0 is: %s"), *MaterialString);

	//M_Buggy_A

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(Mesh);
	
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	VehicleMovementComponent = CreateDefaultSubobject<UChaosVehicleMovementComponent, UChaosWheeledVehicleMovementComponent>(VehicleMovementComponentName);
	VehicleMovementComponent->SetIsReplicated(true); // Enable replication by default
	VehicleMovementComponent->UpdatedComponent = Mesh;
}

void AVehicle::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos)
{
	static FName NAME_Vehicle = FName(TEXT("Vehicle"));

	Super::DisplayDebug(Canvas, DebugDisplay, YL, YPos);
}

class UChaosVehicleMovementComponent* AVehicle::GetVehicleMovementComponent() const
{
	return VehicleMovementComponent;
}

// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();

	VehicleMovementComponent->SetHandbrakeInput(true);

	PreviousLocation = this->GetActorLocation();
	UE_LOG(LogTemp, Display, TEXT("[DISTANCETEST] Starting Location is: %s"), *PreviousLocation.ToString()); 

	auto ActorComponent = this->GetComponentByClass(UWeaponsSystemComponent::StaticClass());
	WeaponsSystemComponent = Cast<UWeaponsSystemComponent>(ActorComponent);
	//Material'/Game/Vehicles/VH_Buggy/Materials/M_Buggy_A.M_Buggy_A'
	//SkeletalMesh'/Game/Vehicles/VH_Buggy/Mesh/SK_Buggy_Vehicle.SK_Buggy_Vehicle'
	/*InitialRotation = TurretMesh->GetRelativeRotation(); */
	//FRotator TestRot = TurretMesh->GetComponentRotation(); 
	//UE_LOG(LogTemp, Display, TEXT("[TESTING] Turret Relative Location is: %s and location %s and rotation: %s %s"), *TurretMesh->GetRelativeLocation().ToString(), *TurretMesh->GetComponentLocation().ToString(),  *TurretMesh->GetComponentRotation().ToString(), *TurretMesh->GetRelativeRotation().ToString());
	//UE_LOG(LogTemp, Display, TEXT("[TESTING]Turret Rotation Pitch: %f, Roll: %f, Yaw: %f"), TestRot.Pitch, TestRot.Roll, TestRot.Yaw);
}

// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("[TANKMOVEMENTTEST] Inside TIck...."),)
	if (!HasReachedDistanceLimit && (isMovingBack || isMovingForw))
	{
		VehicleMovementComponent->SetHandbrakeInput(false);
		FVector CurrentLocation = this->GetActorLocation();
		//UE_LOG(LogTemp, Display, TEXT("[DISTANCETEST] Current Location: %s PreviousLocation: %s"), *CurrentLocation.ToString(), *PreviousLocation.ToString());
		float Distance = FMath::Abs(FVector::Dist(CurrentLocation, PreviousLocation)); 
		//UE_LOG(LogTemp, Display, TEXT("[DISTANCETEST] Difference in distance is: %f"), Distance);
		DistanceTravelled += Distance; 
		//UE_LOG(LogTemp, Display, TEXT("[DISTANCETEST] Distance travelled: %f"), DistanceTravelled);
		PreviousLocation = CurrentLocation;
		if (DistanceTravelled >= TotalAllowedDistance) {
			HasReachedDistanceLimit = true;
		}
	}
	else {
		VehicleMovementComponent->SetHandbrakeInput(true);
	}
	FRotator TestRot = TurretMesh->GetComponentRotation();
	UE_LOG(LogTemp, Display, TEXT("[TESTINGTT]Turret Rotation Pitch: %f, Roll: %f, Yaw: %f"), TestRot.Pitch, TestRot.Roll, TestRot.Yaw);
}

// Called to bind functionality to input
void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UE_LOG(LogTemp, Display, TEXT("[TANKMOVEMENTTEST] SetupPlayerInputComponenet being called......."), )
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AVehicle::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Reverse"), this, &AVehicle::Reverse);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AVehicle::Turn);
	PlayerInputComponent->BindAxis(TEXT("RaiseTurret"), this, &AVehicle::RaiseTurret);
	PlayerInputComponent->BindAxis(TEXT("RotateTurret"), this, &AVehicle::RotateTurretAlt);
	PlayerInputComponent->BindAxis(TEXT("SetTurretRange"), this, &AVehicle::SetTurretRange);
	PlayerInputComponent->BindAxis(TEXT("RotateWeapons"), this, &AVehicle::RotateThroughWeapons);
	PlayerInputComponent->BindAxis(TEXT("SelectWeapon"), this, &AVehicle::SelectWeapon);
	
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AVehicle::FireProjectile);
}

void AVehicle::Reverse(float Value) {

	if (static_cast<int>(Value) == 1) {
		isMovingBack = true;
		//UE_LOG(LogTemp, Display, TEXT("[DISTANCETEST] MOVEBackward VALUE: %f"), Value);
		//VehicleMovementComponent->SetHandbrakeInput(false);
	}
	else {
		isMovingBack = false; 
		//VehicleMovementComponent->SetHandbrakeInput(true);
	}
	if (HasReachedDistanceLimit) {
		Value = 0.0;
		VehicleMovementComponent->SetHandbrakeInput(true);
	}
	VehicleMovementComponent->SetBrakeInput(Value);
}

void AVehicle::MoveForward(float Value)
{
	if (static_cast<int>(Value) == 1) {
		//UE_LOG(LogTemp, Display, TEXT("[DISTANCETEST] MOVEFORWARD VALUE: %f"), Value);
		isMovingForw = true;
		//VehicleMovementComponent->SetHandbrakeInput(false);
	}
	else {
		isMovingForw = false;
		//VehicleMovementComponent->SetHandbrakeInput(true);
	}
	if (HasReachedDistanceLimit) {
		Value = 0.0;
		VehicleMovementComponent->SetHandbrakeInput(true); 
	}
	VehicleMovementComponent->SetThrottleInput(Value);
}

void AVehicle::Turn(float Value)
{
	//FRotator DeltaRotation = FRotator::ZeroRotator;
	//DeltaRotation.Yaw = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * TurnRate;
	//AddActorLocalRotation(DeltaRotation, true);
	VehicleMovementComponent->SetSteeringInput(Value); 
	AddControllerYawInput(Value); 
}

void AVehicle::RaiseTurret(float Value)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Pitch = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * 10;
	//DeltaRotation.Yaw = InitialRotation.Yaw; 
	//DeltaRotation.Roll = InitialRotation.Roll; 
	float CurrentPitch = TurretMesh->GetComponentRotation().Pitch; 
	UE_LOG(LogTemp, Display, TEXT("[TESTING] Current change is: %f and Value is: %f and value is: %f, %d"), DeltaRotation.Pitch, Value, TurretMesh->GetComponentRotation().Pitch, TurretMesh->GetComponentRotation().Pitch == MaxPitch);
	//UE_LOG(LogTemp, Display, TEXT("[TESTING] BOOL TEST: %d %d %d %.25f"), TurretMesh->GetComponentRotation().Pitch > 10.000000, TurretMesh->GetComponentRotation().Pitch == 10.0, TurretMesh->GetComponentRotation().Pitch < 10.0, TurretMesh->GetComponentRotation().Pitch);
	if (DeltaRotation.Pitch != 0 && floor(CurrentPitch) <= MaxPitch && floor(CurrentPitch) >= MinPitch)
	{
		TurretMesh->AddLocalRotation(DeltaRotation);
		UE_LOG(LogTemp, Display, TEXT("[TESTING] Turret Pitch is: %f"), TurretPitch);

		//TurretMesh->SetWorldRotation(DeltaRotation); //Why does this not work properly?

		//if (TurretMesh->GetComponentRotation().Pitch > MaxPitch)
		//{

		//	float ValueToAdjust = -(TurretMesh->GetComponentRotation().Pitch - MaxPitch);
		//	UE_LOG(LogTemp, Display, TEXT("[TESTING] Value to adjust: %f"), ValueToAdjust);
		//	InitialRotation.Add(ValueToAdjust, 0.0f, 0.0f);
		//	TurretMesh->AddLocalRotation(InitialRotation); //FRotator(ValueToAdjust, 0.0f, 0.0f)
		//}
		//if (TurretMesh->GetComponentRotation().Pitch < MinPitch)
		//{
		//	float ValueToAdjust = -(TurretMesh->GetComponentRotation().Pitch - MinPitch);
		//	UE_LOG(LogTemp, Display, TEXT("[TESTING] Value to adjust: %f"), ValueToAdjust);
		//	InitialRotation.Add(ValueToAdjust, 0.0f, 0.0f);
		//	TurretMesh->AddLocalRotation(InitialRotation);
		//}
		UE_LOG(LogTemp, Display, TEXT("[TESTING] Turret Pitch2 is: %f"), CurrentPitch);
	}
	else if (DeltaRotation.Pitch == 0 && CurrentPitch > MaxPitch ) {
		UE_LOG(LogTemp, Display, TEXT("[TESTINGG] Pitch gone higher than maximum"));
		InitialRotation.Add(MaxPitch, 0.0f, 0.0f);
		TurretMesh->AddLocalRotation(FRotator(MaxPitch - CurrentPitch, 0.0f, 0.0f)); 
		//TurretMesh->SetRelativeRotation(FRotator(MaxPitch, TurretMesh->GetComponentRotation().Yaw, TurretMesh->GetComponentRotation().Roll));
	}
	else if (DeltaRotation.Pitch == 0 && CurrentPitch < MinPitch) {
		UE_LOG(LogTemp, Display, TEXT("[TESTINGG] Pitch gone lower than minimum"));
		InitialRotation.Add(MinPitch, 0.0f, 0.0f);
		TurretMesh->AddLocalRotation(FRotator(MinPitch - CurrentPitch, 0.0f, 0.0f)); 
		//TurretMesh->SetRelativeRotation(FRotator(MinPitch, TurretMesh->GetComponentRotation().Yaw, TurretMesh->GetComponentRotation().Roll));
		//TurretMesh->AddLocalRotation(InitialRotation);
	}
	TurretPitch = TurretMesh->GetComponentRotation().Pitch;
}

void AVehicle::RotateTurretAlt(float Value)
{
	if (InitialRotation.IsZero()) {
		InitialRotation = TurretMesh->GetComponentRotation();
		UE_LOG(LogTemp, Display, TEXT("[TESTING] Initial ROtation SET: %f %f %f"), InitialRotation.Pitch, InitialRotation.Roll, InitialRotation.Yaw);
	}
	FHitResult HitResult;
	UGameplayStatics::GetPlayerController(this, 0)->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, false, HitResult);
	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, -1);
	float TankPitch = TurretMesh->GetComponentRotation().Pitch;//GetActorRotation().Pitch;
	float TankRoll = InitialRotation.Roll;//TurretMesh->GetComponentRotation().Roll; 
	UE_LOG(LogTemp, Display, TEXT("[Testing] Turrent Rotation Pitch: %f Roll: %f %f"), TankPitch, TankRoll, InitialRotation.Pitch);
	FVector ToTarget = HitResult.ImpactPoint - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(TankPitch, ToTarget.Rotation().Yaw, 0.0);
	FString PawnName = UGameplayStatics::GetPlayerController(this, 0)->GetPawn()->GetActorNameOrLabel();
	//UE_LOG(LogTemp, Display, TEXT("[BasePawn RotateTankTurret] Turrent Mesh belongs to: %s %s and movement is: %s and distance is: %s"), *TurretMesh->GetOwner()->GetActorNameOrLabel(), *PawnName, *LookAtRotation.ToString(), *ToTarget.ToString());
	if (UKismetMathLibrary::Abs(ToTarget.X) || UKismetMathLibrary::Abs(ToTarget.Y) >= 100)
	{
		TurretMesh->SetWorldRotation(FMath::RInterpTo(TurretMesh->GetComponentRotation(), LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 20.f));
	}
	//UE_LOG(LogTemp, Display, TEXT("[TURRRETTEST] Turret Relative Location is: %s \t and rotation: %s"), *TurretMesh->GetRelativeLocation().ToString(), *TurretMesh->GetRelativeRotation().ToString()); 
}

void AVehicle::HandleDestruction()
{
	//Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}

void AVehicle::FireWeaponChangeEvent()
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
FString AVehicle::ReturnPlayerName()
{

	return TankDetails.Name;
}

void AVehicle::SetTurretRange(float Value)
{

	if (Value == 1.0 && Range < 2.0)
	{
		Range += 0.01;
		if (Range > 2.0)
		{
			Range = 2.0;
		}
	}
	else if (Value == -1.0 && Range > 0.0)
	{
		Range -= 0.01;
		if (Range < 0.0)
		{
			Range = 0.0;
		}
	}
	UE_LOG(LogTemp, Display, TEXT("[RANGE] %f %f %d"), Value, Range, Range > 2.0);

}

void AVehicle::SelectWeapon(float Value)
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


void AVehicle::RotateThroughWeapons(float Value)
{
	//TODO
	if (Value != 0.0)
	{

		//UE_LOG(LogTemp, Display, TEXT("[PROJECTILE] RotateThroughWeapons Value: %f"), Value);
	}

}


void AVehicle::FireProjectile()
{
	/*ProjectileType = 2;*/
	UE_LOG(LogTemp, Display, TEXT("[PROJECTILE] Fire method being overridden, Projectile Type is"));
	if (!IsInTransition)
	{
		UE_LOG(LogTemp, Display, TEXT("[PROJECTILE] IS not in transition"));
		WeaponIndex = WeaponsSystemComponent->ReturnProjectileIndex();
		UE_LOG(LogTemp, Display, TEXT("[PROJECTILE] Fire method being overridden, Projectile Type is: %d"), WeaponIndex);
		Fire(WeaponIndex);
		WeaponsSystemComponent->UpdateProjectileAmmoCount(WeaponIndex);
		IsInTransition = true;
	}
}

void AVehicle::SetPlayerDetails(FPlayerDetails TankDetail)
{
	UE_LOG(LogTemp, Display, TEXT("[PLAYERTEST] Inside SetPlayerDetails: %s %s"), *TankDetail.Name, *TankDetail.Color.ToString());
	TankDetails = TankDetail;
	FString MaterialName = TEXT("Material'/Game/Vehicles/VH_Buggy/Materials/M_Buggy_A.M_Buggy_A'");

	UMaterialInterface* Material = Mesh->GetMaterial(0);
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
	Mesh->SetMaterial(0, DynamicMaterial);
	TurretMesh->SetMaterial(0, DynamicMaterial);
	UE_LOG(LogTemp, Display, TEXT("[PLAYERTEST] ASsigning TANK COLOR IN BEGINPLAY: %s"), *TankDetails.Color.ToString());
	DynamicMaterial->SetVectorParameterValue(TEXT("Paint Color"), TankDetails.Color);
}

void AVehicle::Fire(int ProjectileType)
{
	UE_LOG(LogTemp, Display, TEXT("[PROJECTILE] TESTING WHEN THIS IS CALLED!!!! %d"), ProjectileType);
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters(); 
	SpawnParameters.Instigator = this; 
	SpawnParameters.Owner = this;
	if (ProjectileType == 1)
	{
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation(), SpawnParameters);
		//Projectile->SetOwner(this);
		Projectile->UpdateProjectileSpeed(Range);
	}
	else if (ProjectileType == 2)
	{
		ACannonMissile* ProjectileTwo = GetWorld()->SpawnActor<ACannonMissile>(CannonClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation(), SpawnParameters);
		ProjectileTwo->SetOwner(this);
	}
	else if (ProjectileType == 3)
	{
		ARocketMissile* RocketMissile = GetWorld()->SpawnActor<ARocketMissile>(RocketClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation(), SpawnParameters);
		RocketMissile->SetOwner(this);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("[PROJECTILE] Nothing to fire here: %d"), ProjectileType);
	}
}