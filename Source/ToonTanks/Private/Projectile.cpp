// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include <string>

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Vehicle.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"
#include "CannonMissile.h"
#include "WeaponsSystemComponent.h"
#include "MissileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ProjectileMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMeshComp");
	RootComponent = ProjectileMeshComp;

	ProjectileMoveComp = CreateDefaultSubobject<UMissileMovementComponent>(TEXT("ProjectileMoveComp"));
	ProjectileMoveComp->InitialSpeed = 4000.0f;
	ProjectileMoveComp->MaxSpeed = 8000.0f;

	ParticleSmoke = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSmoke"));
	ParticleSmoke->SetupAttachment(RootComponent); 

	//UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] Cannon/Projectile/Missile Initialized!!! %s"), *GetOwner()->GetClass()->GetName());
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation(), GetActorRotation());
	}
	UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] OnBEginPLay......"));
	ProjectileMeshComp->OnComponentHit.AddDynamic(this, &AProjectile::onHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::onHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] Inside onHit parent!!! %s"), *this->GetName());
	AActor* MyOwner = GetOwner();
	UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] Inside on Hit in Projectile class %s %s %s %s"), *OtherActor->GetActorNameOrLabel(), *OtherActor->GetName(), *MyOwner->GetActorNameOrLabel(), *MyOwner->GetName());
	UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] Checking the class %s %s"), *(MyOwner->GetClass()->GetName()),*(OtherActor->GetClass()->GetName()));
	if (MyOwner == nullptr) {
		Destroy(); 
		return;
	}
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] Other actor is %d %d"), OtherActor != this, OtherActor != MyOwner);
	if (OtherActor && OtherActor != this && OtherActor != MyOwner) //&& OtherActor!= this && OtherActor != MyOwner
	{
		UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] Inside the statement..."));
		UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] Inside judgement day!!! %d"), this->GetClass()->IsChildOf(ACannonMissile::StaticClass()));
	/*	if (MyOwner->GetClass()->IsChildOf(ATower::StaticClass()))
		{
			ATank *Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
			UE_LOG(LogTemp, Warning, TEXT("[Testing] THE OWNER OF THE PROJECTILE IS A TURRET....%s"), *Tank->GetActorNameOrLabel()); 

		}*/
		FVector Velocity = this->ProjectileMeshComp->GetPhysicsLinearVelocity();
		UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] CANNON MISSILE TIME....Speed: %s"), *Velocity.ToString());
		AVehicle* InstigatorTank = Cast<AVehicle>(MyOwner);
		if (this->GetClass()->IsChildOf(ACannonMissile::StaticClass()) && OtherActor->ActorHasTag("Floor"))
		{
			
			FVector CurrentLocation = this->GetActorLocation();
			if (StartLocation.IsZero())
			{
				UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] FVECTOR IS ZERO!!!!"));
				StartLocation = CurrentLocation;
				//LastLocation = CurrentLocation - 1; 
				UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] New Start Location is: %s"), *StartLocation.ToString());
			}

			FVector FutureLocation = CurrentLocation + MovementOffset;
			this->ProjectileMeshComp->SetPhysicsLinearVelocity(Velocity * 0.98);
			UE_LOG(LogTemp, Warning, TEXT("[PROJECTILEROLL] Distance between start and future location: %f"), FVector::Distance(CurrentLocation, LastLocation));
			if (FVector::Distance(StartLocation, CurrentLocation) > 1800 || FVector::Distance(LastLocation, CurrentLocation) == 0)
			{
				UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
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
				SwitchPlayer(); 
				Destroy(); 
			}
			UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] Current Location: %s vs New Location: %s"), *CurrentLocation.ToString(), *FutureLocation.ToString());
			UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] I hit them floors!!!"));



		/*		while (CurrentLocation.X < FutureLocation.X || CurrentLocation.Y < FutureLocation.Y)
				{
					UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] Hasn't reached distance yet!"));
				}*/
			LastLocation = CurrentLocation; 
			
		} else
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
			if (HitParticles)
			{
				UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] SHAKE SHAKE SHAKE THAT FOR ME!"));
				UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
			}
			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation(), GetActorRotation());
			}
			if (HitCameraShakeBaseClass)
			{
				GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeBaseClass);
			}
			if (!MyOwner->GetClass()->IsChildOf(ATower::StaticClass()))
			{
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
				SwitchPlayer(); 
			}
			Destroy();
			
		}

	}
	UE_LOG(LogTemp, Warning, TEXT("[PROJECTILE] End of OnHit!!!"));

}

void AProjectile::SwitchPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("[PLAYERSWITCH] Inside switch player!!!"))
	AToonTanksPlayerController* ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	ToonTanksPlayerController->SetPlayerEnabledState(false);
	FTimerHandle PlayerEnableTimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, &AToonTanksPlayerController::ChangePlayer, false);
	GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, TimerDelegate, 2.0f, false);
}

void AProjectile::CreateProjectileTwo()
{
	ProjectileMoveComp->InitialSpeed = 8000.0f;
	ProjectileMoveComp->MaxSpeed = 8000.0f;
	Damage = 100; 
	Damage = 100; 
}

void AProjectile::UpdateProjectileSpeed(float NewSpeed)
{
	ProjectileMoveComp->Velocity = ProjectileMoveComp->Velocity * NewSpeed;
	//ProjectileMoveComp->UpdateVelocity(ProjectileMoveComp->InitialSpeed * NewSpeed);
	UE_LOG(LogTemp, Display, TEXT("[VELOCITY] Update Projectile Speed %f %s"), ProjectileMoveComp->InitialSpeed, *ProjectileMoveComp->Velocity.ToString())
}

