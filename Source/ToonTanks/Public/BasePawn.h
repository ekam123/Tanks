// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

class UCapsuleComponent;
class USceneComponent;
class UParticleSystem;
class USoundBase;
class UCameraShakeBase; 

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	void HandleDestruction(); 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RotateTurret(FVector LookAtTarget);

	void GetTurretPitch(float Value);

	void RotateTankTurret(float Value); 

	virtual void Fire(int ProjectileType = 0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle")
		float Range = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* BaseMesh;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UCapsuleComponent* CapsuleComp;

	//UPROPERTY(VisibleAnywhere, Category = "Components")
	//	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* TurretMesh;

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

	float MinPitch = -20.0f;
	float MaxPitch = 20.0f; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	TSubclassOf<class AProjectile> ReturnProjectile();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		TSubclassOf<class AProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		TSubclassOf<class AProjectile> CannonClass;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		TSubclassOf<class AProjectile> RocketClass;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		TSubclassOf<class AProjectile> CracklingClass;

};
