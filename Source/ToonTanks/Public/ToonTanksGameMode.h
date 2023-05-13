// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

/**
 * 
 */
 USTRUCT(BlueprintType)
 struct FPlayerDetails
 {
 	GENERATED_BODY()
 		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
 		FString Name;
 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
 		int Index;
 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FLinearColor Color;
 };

class UWindUtils;
//struct FPlayerDetails;

UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ActorDied(AActor* DeadActor, bool KilledByTank);

protected:
	void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
		void GameOver(bool bWonGame);

private:
	class AVehicle* Tank;
	class AToonTanksPlayerController* ToonTanksPlayerController;

	int TargetTowers = 0;
	int GetTargetTowerCount();

	void HandleGameStart();
	void HandleWind();

private:
	UWindUtils* WindUtils;

	float StartDelay = 3.f;
	TArray<FPlayerDetails> Players;

public:
	void ReturnWindOffset(FVector& WindOffset, FVector Direction, float DeltaTime);
	UFUNCTION(BlueprintCallable, Category = Properties)
	void SetPlayers(TArray<FPlayerDetails> CPlayers, TArray<FVector> Locations);

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		TSubclassOf<class AVehicle> TankClass; //TSubclassOf<class ATank> TankClass;
	TArray<AActor*> Tanks;
};
