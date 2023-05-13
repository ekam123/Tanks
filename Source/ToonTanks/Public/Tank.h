// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "ToonTanksGameMode.h"
#include "Tank.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWeaponsSystemComponent;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChanged, int, WeaponIndex);
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	void HandleDestruction();

	void SetPlayerDetails(FPlayerDetails TankDetail);

	//UPROPERTY(BlueprintAssignable)
	//	FOnWeaponChanged OnWeaponChanged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APlayerController* GetTankPlayerController() const { return TankPlayerController; }

	bool bAlive = true;
	bool IsInTransition = false; 
	void FireWeaponChangeEvent(); 
	UFUNCTION(BlueprintCallable, Category = Properties)
	FString ReturnPlayerName(); 

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere)
		float Speed = 200.0f;
	UPROPERTY(EditAnywhere)
		float TurnRate = 25.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle", meta = (AllowPrivateAccess = "true"))
		float TotalAllowedDistance = 50000.0f; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = "true"))
		float DistanceTravelled = 0.f;
	//UPROPERTY(BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = "true"))
		int WeaponIndex = 1;


	APlayerController* TankPlayerController;
	UWeaponsSystemComponent* WeaponsSystemComponent;
	bool HasReachedDistanceLimit = false;
	TArray<int> WeaponTypes;
	FPlayerDetails TankDetails; 

	//UPROPERTY(EditAnywhere, Category = "Battle")
	//FComponentReference HealthComponentReference;


	void MoveForward(float Value);
	void Turn(float Value);
	void RaiseTurret(float Value);
	void RotateTurretAlt(float Value);
	void SetTurretRange(float Value);
	void RotateThroughWeapons(float Value); 
	void FireProjectile();
	void SelectWeapon(float Value);

	virtual void Fire(int ProjectileType = 0) override;

	
};



