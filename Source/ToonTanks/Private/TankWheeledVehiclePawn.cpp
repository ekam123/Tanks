// Fill out your copyright notice in the Description page of Project Settings.


#include "TankWheeledVehiclePawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"

ATankWheeledVehiclePawn::ATankWheeledVehiclePawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	////SpringArmComp->SetupAttachment(RootComponent);
	//this->GetMesh()->SetupAttachment(SpringArmComp); 

	//CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	//CameraComp->SetupAttachment(SpringArmComp);

	////this->GetMesh()->BodyInstance.bSimulatePhysics = false;
	////this->GetMesh()->BodyInstance.bNotifyRigidBodyCollision = true;
	////this->GetMesh()->BodyInstance.bUseCCD = true;
	////this->GetMesh()->bBlendPhysics = true;
	////this->GetMesh()->SetGenerateOverlapEvents(true);
	////this->GetMesh()->SetCanEverAffectNavigation(false);

	///*this->GetMesh() = CreateDefaultSubobject<USkeletalMeshComponent>(VehicleMeshComponentName);*/
	//this->GetMesh()->SetCollisionProfileName(UCollisionProfile::Vehicle_ProfileName);
	//this->GetMesh()->BodyInstance.bSimulatePhysics = false;
	//this->GetMesh()->BodyInstance.bNotifyRigidBodyCollision = true;
	//this->GetMesh()->BodyInstance.bUseCCD = true;
	//this->GetMesh()->bBlendPhysics = true;
	//this->GetMesh()->SetGenerateOverlapEvents(true);
	/*this->GetMesh()->SetCanEverAffectNavigation(false);*/
	////RootComponent = Mesh;

	////VehicleMovementComponent = CreateDefaultSubobject<UChaosVehicleMovementComponent, UChaosWheeledVehicleMovementComponent>(VehicleMovementComponentName);
	//this->GetVehicleMovement()->SetIsReplicated(true); // Enable replication by default
	////VehicleMovementComponent->UpdatedComponent = Mesh;
}

void ATankWheeledVehiclePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATankWheeledVehiclePawn::MoveForward);
	//PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATankWheeledVehiclePawn::Turn);
}

void ATankWheeledVehiclePawn::BeginPlay()
{

	bool IsSimulatingPhysics = this->GetMesh()->IsSimulatingPhysics();
	bool IsGravityEnabled = this->GetMesh()->IsGravityEnabled(); 
	UE_LOG(LogTemp, Display, TEXT("[TANKMOVEMENTTEST] Forward Vector: %d %d"), IsSimulatingPhysics, IsGravityEnabled);
}

void ATankWheeledVehiclePawn::MoveForward(float Value)
{

}

void ATankWheeledVehiclePawn::Turn(float Value)
{
}
