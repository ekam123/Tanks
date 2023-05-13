// Fill out your copyright notice in the Description page of Project Settings.


#include "CracklingMissile.h"

#include "Vehicle.h"
#include "Tower.h"
#include "WeaponsSystemComponent.h"
#include "Kismet/GameplayStatics.h"


void ACracklingMissile::onHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr) {
		Destroy();
		return;
	}
	if (OtherActor && OtherActor != this && OtherActor != MyOwner && !OtherActor->GetClass()->IsChildOf(ACracklingMissile::StaticClass()))
	{
		auto MyOwnerInstigator = MyOwner->GetInstigatorController();
		auto DamageTypeClass = UDamageType::StaticClass();
		AVehicle* InstigatorTank = Cast<AVehicle>(MyOwner);
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		UE_LOG(LogTemp, Warning, TEXT("[SCORE] CRACKLING MISSILE ON HIT... %s, Damage is: %f and the owner is: %s"), *this->GetName(), Damage, *MyOwner->GetActorNameOrLabel());
		if (OtherActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("[SCORE] Instigator Tank: %s     OtherActor: %s   "), *InstigatorTank->GetActorNameOrLabel(), *OtherActor->GetActorNameOrLabel());
		}

		if (InstigatorTank && (Cast<AVehicle>(OtherActor) || Cast<ATower>(OtherActor)))
		{
			UE_LOG(LogTemp, Display, TEXT("[SCORE] Inside CAsting"));
			UWeaponsSystemComponent* WeaponComponent = Cast<UWeaponsSystemComponent>(InstigatorTank->GetComponentByClass(UWeaponsSystemComponent::StaticClass()));
			if (WeaponComponent)
			{
				UE_LOG(LogTemp, Display, TEXT("[SCORE] KILLED BY TANK IS TRUE: %s"), *WeaponComponent->GetOwner()->GetActorNameOrLabel());
				WeaponComponent->UpdateScore(Damage);
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("[SCORE] Unable to retrieve/cast UHealthComponent"));
			}
		}
		
		Destroy();
	}
}



