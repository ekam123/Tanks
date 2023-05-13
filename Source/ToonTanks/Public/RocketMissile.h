// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "RocketMissile.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ARocketMissile : public AProjectile
{
	GENERATED_BODY()

private:
	float Damage = 30.0f;
	UPROPERTY(EditAnywhere)
	int CracklingMissileCount = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		TSubclassOf<class ACracklingMissile> ProjectileClass;

	FTimerHandle SwitchPlayerTimerHandle;

private:
	void onHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
};
