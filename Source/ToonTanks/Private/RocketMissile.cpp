// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketMissile.h"
#include "Vehicle.h"
#include "Tower.h"
#include "WeaponsSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CracklingMissile.h"
#include "CannonMissile.h"

void ARocketMissile::onHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] Inside onHit child!!! %s"), *this->GetName());
	AActor* MyOwner = GetOwner();
	UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] Inside on Hit in Projectile class %s %s %s %s"), *OtherActor->GetActorNameOrLabel(), *OtherActor->GetName(), *MyOwner->GetActorNameOrLabel(), *MyOwner->GetName());
	UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] Checking the class %s %s"), *(MyOwner->GetClass()->GetName()), *(OtherActor->GetClass()->GetName()));
	if (MyOwner == nullptr) {
		Destroy();
		return;
	}
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{

		AVehicle* InstigatorTank = Cast<AVehicle>(MyOwner);

			//UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
			//if (HitParticles)
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] SHAKE SHAKE SHAKE THAT FOR ME!"));
			//	UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
			//}
			//if (HitSound)
			//{
			//	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation(), GetActorRotation());
			//}
			//if (HitCameraShakeBaseClass)
			//{
			//	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeBaseClass);
			//}

	/*			if (InstigatorTank && (Cast<ATank>(OtherActor) || Cast<ATower>(OtherActor)))
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
				}*/
				//SwitchPlayer(); 
			//GetWorldTimerManager().SetTimer(SwitchPlayerTimerHandle, this, &ARocketMissile::SwitchPlayer, 3.0f, false);
			Destroy();
			for (int i = 0; i < CracklingMissileCount; ++i)
			{
				//auto test = InstigatorTank->ReturnProjectile(); 
				DrawDebugSphere(GetWorld(), Hit.GetComponent()->GetComponentLocation(), 50, 50, FColor::Red, false, 5);
				FActorSpawnParameters SpawnParameters;
				SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				FRotator OppRotation = Hit.GetComponent()->GetComponentRotation();
				FRotator AltRotation = FRotator(0, 0, 0); 
				ACracklingMissile* MiniMissile = GetWorld()->SpawnActor<ACracklingMissile>(InstigatorTank->CracklingClass, Hit.ImpactPoint, AltRotation, SpawnParameters);
				if (MiniMissile)
				{
					MiniMissile->SetOwner(MyOwner);
					UE_LOG(LogTemp, Warning, TEXT("[MINIMISSILE] Mini Missile Speed: %s %s"), *MiniMissile->GetVelocity().ToString(), *this->GetVelocity().ToString()); //Hit.GetComponent()->GetComponentLocation()
				}
			}
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([&]
				{
					UE_LOG(LogTemp, Warning, TEXT("[PLAYERSWITCH] This text will appear in the console 3 seconds after execution"));
					SwitchPlayer(); 
				});
			GetWorld()->GetTimerManager().SetTimer(SwitchPlayerTimerHandle, TimerDelegate, 3, false);
	}
	UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] Inside judgement day!!!"));
}
