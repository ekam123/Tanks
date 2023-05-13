// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponsSystemComponent.generated.h"

USTRUCT(BlueprintType)
struct FWeaponProperties
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		int Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		int TotalAmmo = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		int RemainingAmmo = 100;
	bool HasAmmo = true; 

	void UpdateAmmoCount() {
		if (RemainingAmmo > 0)
		{
			RemainingAmmo -= 1;
		}
		UE_LOG(LogTemp, Display, TEXT("[WeaponsSystem] Current Ammo Count: %d"), RemainingAmmo);
	}

	int GetAmmoCount() {
		return RemainingAmmo;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChangedTest, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoCountChanged, int, NewCount, int, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChanged, int, WeaponIndex);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UWeaponsSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponsSystemComponent();

	void UpdateScore(int DeltaScore); 
	void UpdateProjectileAmmoCount(int Index);
	void SelectProjectile(int Index); 
	int ReturnProjectileIndex(); 
	void BroadcastProjectileAmmoCounts(); 
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(BlueprintAssignable)
	FOnScoreChangedTest OnScoreChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAmmoCountChanged OnAmmoCountChanged;
	UPROPERTY(BlueprintAssignable)
	FOnWeaponChanged OnWeaponChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	float Score = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = "true"))
	int WeaponIndex = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	TArray<FWeaponProperties> Weapons; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FWeaponProperties DefaultProjectile;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FWeaponProperties RollingProjectile;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FWeaponProperties ArtilleryProjectile;


};