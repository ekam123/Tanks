// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponsSystemComponent.h"

// Sets default values for this component's properties
UWeaponsSystemComponent::UWeaponsSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UWeaponsSystemComponent::UpdateScore(int DeltaScore)
{
	
	Score += DeltaScore;
	OnScoreChanged.Broadcast(Score); 
}

void UWeaponsSystemComponent::UpdateProjectileAmmoCount(int Index)
{
	switch (Index) {
	case 1:
		DefaultProjectile.UpdateAmmoCount(); 
		OnAmmoCountChanged.Broadcast(DefaultProjectile.GetAmmoCount(), Index);
		break;
	case 2:
		RollingProjectile.UpdateAmmoCount(); 
		OnAmmoCountChanged.Broadcast(RollingProjectile.GetAmmoCount(), Index);
		break;
	case 3:
		ArtilleryProjectile.UpdateAmmoCount();
		OnAmmoCountChanged.Broadcast(ArtilleryProjectile.GetAmmoCount(), Index);
		break;
	default:
		UE_LOG(LogTemp, Display, TEXT("[WeaponsSystem] Inside Default of Switch Statement"));
	}

}

void UWeaponsSystemComponent::BroadcastProjectileAmmoCounts()
{

	OnAmmoCountChanged.Broadcast(DefaultProjectile.GetAmmoCount(), DefaultProjectile.Index);

	OnAmmoCountChanged.Broadcast(RollingProjectile.GetAmmoCount(), RollingProjectile.Index);

	OnAmmoCountChanged.Broadcast(ArtilleryProjectile.GetAmmoCount(), ArtilleryProjectile.Index);
}

void UWeaponsSystemComponent::SelectProjectile(int Index)
{
	UE_LOG(LogTemp, Display, TEXT("[WEAPONINDEX] RotateThroughWeapons WeaponSystemsComponent Value: %d %d"), Index, WeaponIndex);
	
	//if (Index != WeaponIndex) {
		WeaponIndex = Index;
		//UE_LOG(LogTemp, Display, TEXT("[PROJECTILE] RotateThroughWeapons Value: %d"), WeaponIndex);
		OnWeaponChanged.Broadcast(WeaponIndex);
		//UE_LOG(LogTemp, Display, TEXT("[PROJECTILE] RotateThroughWeapons Value: %f"), Value);
	//}
}

int UWeaponsSystemComponent::ReturnProjectileIndex() 
{
	return WeaponIndex; 
}

// Called when the game starts
void UWeaponsSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	DefaultProjectile.Index = 1;
	RollingProjectile.Index = 2;
	ArtilleryProjectile.Index = 3; 
	
}


// Called every frame
void UWeaponsSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

