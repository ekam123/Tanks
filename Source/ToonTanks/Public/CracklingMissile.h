// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "CracklingMissile.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ACracklingMissile : public AProjectile
{
	GENERATED_BODY()

private:
	bool HasHitSurface = false;
	int CanBeDestroyed = 20;

private:
	void onHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
};
