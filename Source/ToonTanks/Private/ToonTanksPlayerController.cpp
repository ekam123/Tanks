// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Vehicle.h"

void AToonTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
	UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] Current Set Player Enabled State: %d"), bPlayerEnabled);
	if (bPlayerEnabled)
	{
		GetPawn()->EnableInput(this); 
	} else
	{
		GetPawn()->DisableInput(this);
	}
	bShowMouseCursor = bPlayerEnabled;
	UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] Reached end up SetPlayerEnabledState"));

}

void AToonTanksPlayerController::ChangePlayer(bool TankDestroyed)
{
	int NumOfTanks = Tanks.Num();
	UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] Number of Tanks, In CHANGEPLAYER....%d"), NumOfTanks);
	if (NumOfTanks > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[TANKDEATH] Inside change player function, current player is: %d and indexDeadTank is: %d and numofplayers: %d and NumofTanks: %d"), CurrentPlayer, IndexOfKilledTank, NumOfPlayers, NumOfTanks);

		if (NumOfPlayers > NumOfTanks)
		{
			NumOfPlayers -= 1;
			if (CurrentPlayer == NumOfPlayers && CurrentPlayer >= 0)
			{
				CurrentPlayer = 0;
			}
			if (IndexOfKilledTank >= CurrentPlayer && NumOfTanks > 1 && !TankDestroyed)
			{
				UE_LOG(LogTemp, Warning, TEXT("[TANKDEATH] Killed Tank in front, need to move to next tank"));
				if (CurrentPlayer+1 >= NumOfTanks)
				{
					CurrentPlayer = 0;
				} else
				{
					CurrentPlayer += 1;
				}
				
			}
		}
		else
		{
			if (CurrentPlayer >= NumOfPlayers - 1)
			{
				UE_LOG(LogTemp, Warning, TEXT("[TANKDEATH] Current Player is >= NumOfPlayers - 1"));
				CurrentPlayer = 0;
			}
			else
			{
				CurrentPlayer += 1;
			}
		}

		APawn* NewTarget = Cast<APawn>(Tanks[CurrentPlayer]);
		AVehicle* TargetTank = Cast<AVehicle>(Tanks[CurrentPlayer]);
		TargetTank->IsInTransition = false; 
		TargetTank->FireWeaponChangeEvent(); 
		this->Possess(NewTarget);
		this->SetPlayerEnabledState(true);
		UE_LOG(LogTemp, Warning, TEXT("[TANKDEATH] Current Player is: %d %s NumOfPlayers: %d"), CurrentPlayer, *NewTarget->GetActorNameOrLabel(), NumOfPlayers);
	}
}

AToonTanksPlayerController::AToonTanksPlayerController()
{
	UE_LOG(LogTemp, Warning, TEXT("[SPAWNTESTING] TOONTANKSPLAYERCONTROLLER INITIALIZED!!!"));
	//UGameplayStatics::GetAllActorsOfClass(this, ATank::StaticClass(), Tanks);
	//NumOfPlayers = Tanks.Num(); 
	UE_LOG(LogTemp, Warning, TEXT("[SPAWNTESTING] Number of elements in array: %d"), Tanks.Num());

}

void AToonTanksPlayerController::RemovePlayerFromGame(AActor* DeadActor)
{
	UE_LOG(LogTemp, Display, TEXT("[TANKDEATH] Removing dead player, old size of tanks: %d"), Tanks.Num());
	//for (auto x : Tanks)
	//{
	//	bool t = DeadActor == x;
	//	UE_LOG(LogTemp, Display, TEXT("[TANKDEATH] TESTING TESTING TESTING TESTING...%s, %d"), *x->GetActorNameOrLabel(), t);
	//}
	if (Tanks.Find(DeadActor) != INDEX_NONE)
	{
		IndexOfKilledTank = Tanks.Find(DeadActor);
		UE_LOG(LogTemp, Display, TEXT("[TANKDEATH] Dead Actor is still present in array, removing now..."));
		AVehicle* DeadTank = Cast<AVehicle>(DeadActor);
		Tanks.Remove(DeadTank);
		//if (CurrentPlayer != 0)
		//{
		//	CurrentPlayer -= 1;

		//}

		UE_LOG(LogTemp, Display, TEXT("[TANKDEATH] New Size of Tank Array is: %d, current player is: %d"), Tanks.Num(), CurrentPlayer);
	} else
	{
		UE_LOG(LogTemp, Display, TEXT("[TANKDEATH]Tank: %s has already been removed from the game...."), *DeadActor->GetActorNameOrLabel());
	}
}

void AToonTanksPlayerController::SetPlayers(FPlayerDetails CPlayer, FVector Location)
{
	UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] Inside SetPlayers AToonTanksPlayerController...."));
	//FActorSpawnParameters SpawnInfo; 
	//SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; 
	//ATank* NewTank = GetWorld()->SpawnActor<ATank>(TankClass, Location, FRotator(0, 0, 0));
	//NewTank->SetPlayerDetails(CPlayer);
	//NewTank->SetOwner(this);
	//Tanks.Add(NewTank); 
	//UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] Number of Tanks: %d"), Tanks.Num());
	//if (Tanks.Num() == 1) {
	//	UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] Possessing new tank"));
	//	this->Possess(NewTank);
	//}
	////TestTanks.Add(NewTank); 
	//auto GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//Tanks = GameMode->Tanks;
}

void AToonTanksPlayerController::SetNumberOfPlayers(int NumberOfPlayers)
{
	NumOfPlayers = NumberOfPlayers;
	UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] TOONTANKSPLAYERCONTROLLER: Num of Players: %d"), NumOfPlayers);
}

void AToonTanksPlayerController::SetTanks(TArray<AActor*> PlayerTanks)
{
	UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] Setting TANKS"));
	Tanks = PlayerTanks; 
	for (auto it = Tanks.begin(); it != Tanks.end(); ++it) {
		auto& element = *it;
		UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] REsult: %s"), *element->GetClass()->GetName());
	}
	UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] Number of Tanks: %d"), Tanks.Num());
}

