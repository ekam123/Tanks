// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ToonTanksGameMode.h"
#include "ToonTanksPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetPlayerEnabledState(bool bPlayerEnabled);

	void ChangePlayer(bool TankDestroyed);

	AToonTanksPlayerController();

	void RemovePlayerFromGame(AActor* DeadActor);
	UFUNCTION(BlueprintCallable, Category = Properties)
	void SetPlayers(FPlayerDetails CPlayer, FVector Location);

	/*UFUNCTION(BlueprintCallable, Category = Properties)*/
	void SetNumberOfPlayers(int NumberOfPlayers);

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TSubclassOf<class AVehicle> TankClass;

	void SetTanks(TArray<AActor*> PlayerTanks); 

public:
	int IndexOfKilledTank = -1; 

private:
	int CurrentPlayer = 0;
	TArray<AActor*> Tanks;
	int NumOfPlayers = 0;
	int NumberOfTanks = 0;
	TArray<AActor*> TestTanks; 

};
