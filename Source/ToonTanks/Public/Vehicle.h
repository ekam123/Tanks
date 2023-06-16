// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ToonTanksGameMode.h"
#include "Vehicle.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWeaponsSystemComponent;

UCLASS(abstract, config = Game, BlueprintType) //abstract, config = Game, BlueprintType
class TOONTANKS_API AVehicle : public APawn
{
	GENERATED_UCLASS_BODY()

public:
	// Sets default values for this pawn's properties
	//AVehicle();

	bool bAlive = true;
	bool IsInTransition = false;
	void FireWeaponChangeEvent();

	UFUNCTION(BlueprintCallable, Category = Properties)
		FString ReturnPlayerName();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle")
		float Range = 1.0f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleDestruction();

	void SetPlayerDetails(FPlayerDetails TankDetail);

	APlayerController* GetTankPlayerController() const { return TankPlayerController; }

private:
	UPROPERTY(Category = Vehicle, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Mesh;

	/** vehicle simulation component */
	UPROPERTY(Category = Vehicle, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UChaosVehicleMovementComponent* VehicleMovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere)
		float Speed = 200.0f;
	UPROPERTY(EditAnywhere)
		float TurnRate = 60.0f; //25.0

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle", meta = (AllowPrivateAccess = "true"))
		float TotalAllowedDistance = 2000.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = "true"))
		float DistanceTravelled = 0.f;
	//UPROPERTY(BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = "true"))
	int WeaponIndex = 1;

	APlayerController* TankPlayerController;
	UWeaponsSystemComponent* WeaponsSystemComponent;
	bool HasReachedDistanceLimit = false;
	TArray<int> WeaponTypes;
	FPlayerDetails TankDetails;
	FVector PreviousLocation; 

	FRotator InitialRotation; 
	bool isMovingForw = false; 
	bool isMovingBack = false; 

private:
	void MoveForward(float Value);
	void Turn(float Value);
	void RaiseTurret(float Value);
	void RotateTurretAlt(float Value);
	void SetTurretRange(float Value);
	void RotateThroughWeapons(float Value);
	void FireProjectile();
	void SelectWeapon(float Value);
	void Reverse(float Value); 

	void Fire(int ProjectileType = 0);

	FString ReturnDirectionStringFromYaw(float Yaw);
	

public:
	/** Name of the MeshComponent. Use this name if you want to prevent creation of the component (with ObjectInitializer.DoNotCreateDefaultSubobject). */
	static FName VehicleMeshComponentName;

	/** Name of the VehicleMovement. Use this name if you want to use a different class (with ObjectInitializer.SetDefaultSubobjectClass). */
	static FName VehicleMovementComponentName;

	/** Util to get the wheeled vehicle movement component */
	class UChaosVehicleMovementComponent* GetVehicleMovementComponent() const;

	//~ Begin AActor Interface
	virtual void DisplayDebug(class UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos) override;
	//~ End Actor Interface

	/** Returns Mesh subobject **/
	class USkeletalMeshComponent* GetMesh() const { return Mesh; }

	/** Returns VehicleMovement subobject **/
	class UChaosVehicleMovementComponent* GetVehicleMovement() const { return VehicleMovementComponent; }

	TSubclassOf<class AProjectile> ReturnProjectile();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		TSubclassOf<class AProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		TSubclassOf<class AProjectile> CannonClass;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		TSubclassOf<class AProjectile> RocketClass;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		TSubclassOf<class AProjectile> CracklingClass;

private: 
	UPROPERTY(VisibleAnywhere, Category = "Components")
		USceneComponent* ProjectileSpawnPoint;


	UPROPERTY(EditAnywhere, Category = "Combat")
		UParticleSystem* Destroyed;

	UPROPERTY(EditAnywhere, Category = "Combat")
		USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;

	UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		float TurretPitch = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		FString TurretDirection;

	float MinPitch = -20.0f;
	float MaxPitch = 20.0f;
};
