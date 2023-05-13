// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UHealthComponent*, OwningActor, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnScoreChanged, AActor*, InstigatorActor, UHealthComponent*, OwningActor, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChangedTest, int, WeaponIndex);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	float Health = 0.f;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
		FOnScoreChanged OnScoreChanged;
	UPROPERTY(BlueprintAssignable)
	FOnWeaponChangedTest OnWeaponChangedTest; 

	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser);

	class AToonTanksGameMode* ToonTanksGameMode;


	//TESTING OUT SCORE PROPERTIES HERE AND THEN CREATE SEPERATE COMPONENT
	//UPROPERTY(BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = "true"))
	//float PlayerScore = 0.0f;

public:
	float PlayerScore = 0.0f;
	void UpdatePlayerScore(); 
};
