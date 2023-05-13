// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanksGameMode.h"
#include "Vehicle.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth; 
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
	ToonTanksGameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this)); 
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0.f || Health <= 0)return;
	Health -= Damage;
	OnHealthChanged.Broadcast(nullptr, this, Health, Damage);
	OnWeaponChangedTest.Broadcast(1);
	auto InstigatorPawn = Instigator->GetPawn();//this->GetComponentByClass(UHealthComponent::StaticClass());
	AVehicle* InstigatorTank = Cast<AVehicle>(InstigatorPawn);
	if (InstigatorTank)
	{
		UE_LOG(LogTemp, Display, TEXT("[SCORE] Inside CAsting"));
		UHealthComponent* InsHealthComponent = Cast<UHealthComponent>(InstigatorTank->GetComponentByClass(UHealthComponent::StaticClass()));
		if (InsHealthComponent)
		{
			UE_LOG(LogTemp, Display, TEXT("[SCORE] KILLED BY TANK IS TRUE: %s"), *InsHealthComponent->GetOwner()->GetActorNameOrLabel());
			InsHealthComponent->PlayerScore += Damage;
			OnScoreChanged.Broadcast(DamageCauser->GetOwner()->GetOwner(), this, InsHealthComponent->PlayerScore, Damage);
		} else
		{
			UE_LOG(LogTemp, Display, TEXT("[SCORE] Unable to retrieve/cast UHealthComponent"));
		}
	}

	UE_LOG(LogTemp, Display, TEXT("[SCORE] Score for tank: %s %s is: %f, DamagedActor is: %s"), *DamageCauser->GetActorNameOrLabel(), *Instigator->GetPawn()->GetActorNameOrLabel(),PlayerScore, *DamagedActor->GetActorNameOrLabel());
	

	//if (Health < 0)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("[TANKDEATH] THis SUCKER IS SUPPSOED TO BE ALREADY DEAD!!!!"));
	//}
	if (Health <= 0.f && ToonTanksGameMode)
	{
		UE_LOG(LogTemp, Display, TEXT("[SCORE] Damage Causer is: %s, damage receiver is: %s"), *DamageCauser->GetOwner()->GetActorNameOrLabel(), *DamagedActor->GetActorNameOrLabel());
		UE_LOG(LogTemp, Display, TEXT("[SCORE] Casting damage causer to AVehicle: %d %d"), DamageCauser->GetOwner(), DamageCauser->GetOwner()->GetClass()->IsChildOf(AVehicle::StaticClass()));
		if (DamageCauser->GetOwner()->GetClass()->IsChildOf(AVehicle::StaticClass()))
		{
			UE_LOG(LogTemp, Display, TEXT("[SCORE] KILLED BY TANK IS TRUE"));
			ToonTanksGameMode->ActorDied(DamagedActor, true);
		} else
		{
			UE_LOG(LogTemp, Display, TEXT("[SCORE] DAmage Causer is not child class AVehicle"));
			ToonTanksGameMode->ActorDied(DamagedActor, false);
		}
		
	
	}

	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);


}

void UHealthComponent::UpdatePlayerScore()
{
	OnScoreChanged.Broadcast(nullptr, this, PlayerScore, 0);
}

