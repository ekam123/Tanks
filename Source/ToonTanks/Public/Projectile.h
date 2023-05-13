// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class UParticleSystem;
class UParticleSystemComponent;
class USoundBase;
class UCameraShakeBase;
class UMissileMovementComponent; 

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
		UStaticMeshComponent* ProjectileMeshComp;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float testThis = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UMissileMovementComponent* ProjectileMoveComp;

	UPROPERTY(EditAnywhere, Category = "Combat")
		UParticleSystem* HitParticles;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
		UParticleSystemComponent* ParticleSmoke;

	UPROPERTY(EditAnywhere, Category = "Combat")
		USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<UCameraShakeBase> HitCameraShakeBaseClass; 

	UPROPERTY(EditAnywhere, Category = "Combat")
		FVector StartLocation;
	UPROPERTY(EditAnywhere, Category = "Combat")
		FVector MovementOffset = FVector(200, 200, 0);

	UFUNCTION()
	virtual void onHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:

	UPROPERTY(EditAnywhere)
		float Damage = 30.f;
	//UPROPERTY(EditAnywhere)
	//	float InitialSpeed = 4000.f; 

	void SwitchPlayer(); 

public:
	void CreateProjectileTwo();
	void UpdateProjectileSpeed(float NewSpeed);

private:
	FVector LastLocation = FVector(0, 0, 0);
 
};
