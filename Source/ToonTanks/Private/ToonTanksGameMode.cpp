// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Vehicle.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"
#include "WindUtils.h"
#include "WindComponent.h"


void AToonTanksGameMode::ActorDied(AActor* DeadActor, bool KilledByTank)
{
	AVehicle* CurrentTank = Cast<AVehicle>(DeadActor);
	if (DeadActor == Cast<AVehicle>(DeadActor)) //Tank
	{
		UE_LOG(LogTemp, Display, TEXT("[TANKDEATH] OKAY< DEAD TANK IS: %s"),*CurrentTank->GetActorNameOrLabel());
		CurrentTank->HandleDestruction();  //Tank->HandleDestruction();
		if (ToonTanksPlayerController)
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false);
			ToonTanksPlayerController->RemovePlayerFromGame(DeadActor);
			FTimerHandle PlayerEnableTimerHandle;
			FTimerDelegate TimerDelegate;
			if (!KilledByTank)
			{
				UE_LOG(LogTemp, Display, TEXT("[TANKDEATH] Tank wasn't killed by tank: %s"), *CurrentTank->GetActorNameOrLabel());
				TimerDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, &AToonTanksPlayerController::ChangePlayer, true);
				GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, TimerDelegate, 3.0f, false);
			}
		/*	else
			{
				UE_LOG(LogTemp, Display, TEXT("[TANKDEATH] HE WAS KILLED BY A TANK"));
				TimerDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, &AToonTanksPlayerController::ChangePlayer, false);
			}*/
			

		}
		//GameOver(false); 
	} else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
	{
		DestroyedTower->HandleDestruction();
		TargetTowers -=1;
		if (TargetTowers == 0)
		{
			//GameOver(true); 
		}
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Something else is going on here!!!"));
	}
}


void AToonTanksGameMode::ReturnWindOffset(FVector& WindOffset, FVector Direction, float DeltaTime)
{
	auto ActorComponent = this->GetComponentByClass(UWindComponent::StaticClass());
	UWindComponent* WindComponent = Cast<UWindComponent>(ActorComponent);
	if (WindComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("[WIND] GOT WIND COMPONENT INSIED TOONTANKSGAMEMODE"));
		WindComponent->CalculateWindOffset(WindOffset, Direction, DeltaTime);
	}

}

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] BEGIN PLAY TOONTANKSGAMEMODE!!!!"));
	
	//auto TESTY = UGameplayStatics::GetPlayerController(this, 0);
	UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] BEGIN PLAY TOONTANKSGAMEMODE a;rogjt!!!!: %s"), *ToonTanksPlayerController->GetClass()->GetName());
	//WindUtils = NewObject<UWindUtils>();
	for (auto it = Players.begin(); it != Players.end(); ++it) {
		auto& element = *it;
		UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] REsult FRom onBeginPlay: %s"), *element.Name);
	}
}

void AToonTanksGameMode::HandleGameStart()
{
	TargetTowers = GetTargetTowerCount(); 
	//Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] Inside HandleGameStart")); 
	
	StartGame();

	if (ToonTanksPlayerController)
	{
		UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] Inside handle game start, ToonTanksPlayerController exists"));
		ToonTanksPlayerController->SetNumberOfPlayers(Tanks.Num());
		ToonTanksPlayerController->SetTanks(Tanks);
		ToonTanksPlayerController->SetPlayerEnabledState(false);
		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, &AToonTanksPlayerController::SetPlayerEnabledState, true);
		GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, TimerDelegate, StartDelay, false);
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] ToonTanksPlayer controller is null"));
	}
}

void AToonTanksGameMode::HandleWind()
{
	UE_LOG(LogTemp, Display, TEXT("[WIND] Inside HandleWind:"))

	auto ActorComponent = this->GetComponentByClass(UWindComponent::StaticClass());
	UWindComponent* WindComponent = Cast<UWindComponent>(ActorComponent);
	if (WindComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("[WIND] GOT WIND COMPONENT INSIED TOONTANKSGAMEMODE"));
		WindComponent->HandleWind();
	}

	
}

int AToonTanksGameMode::GetTargetTowerCount()
{
	TArray<AActor*> Towers; 
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(),Towers);
	return Towers.Num(); 
}

void AToonTanksGameMode::SetPlayers(TArray<FPlayerDetails> CPlayers, TArray<FVector> Locations) {
	UE_LOG(LogTemp, Display, TEXT("[PLAYERTEST] SETPLAYERS CALLED..."));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	Players = CPlayers; 
	int i = 0;
	for (auto it = CPlayers.begin(); it != CPlayers.end(); ++it) {
		UE_LOG(LogTemp, Display, TEXT("[PLAYERTEST] AUTO ... %d"), CPlayers.Num());
		auto& element = *it;
		auto Location = Locations[i]; 
		UE_LOG(LogTemp, Display, TEXT("[PLAYERTEST] REsult: %s %s %d %s"), *element.Name, *Location.ToString(), i, *element.Color.ToString());
		FActorSpawnParameters SpawnInfo; 
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; 
		AVehicle* NewTank = GetWorld()->SpawnActor<AVehicle>(TankClass, Location, FRotator(0, 0, 0), SpawnInfo);
		NewTank->SetPlayerDetails(element);
		NewTank->SetOwner(this);
		Tanks.Add(NewTank); 
		UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] Number of Tanks: %d"), Tanks.Num());
		if (Tanks.Num() == 1) {
			UE_LOG(LogTemp, Display, TEXT("[SPAWNTESTING] Possessing new tank"));
			ToonTanksPlayerController->Possess(NewTank);
		}
		i += 1; 
//TestTanks.Add(NewTank); 
	}
	HandleGameStart();
}