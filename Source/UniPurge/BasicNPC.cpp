// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicNPC.h"

// Sets default values
ABasicNPC::ABasicNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	currentWaypoint = 0;
	disabled = false;
	MaxDistance = 7500.0f;

}

// Called when the game starts or when spawned
void ABasicNPC::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABasicNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//sumDelta += DeltaTime;
	//if (sumDelta >= 1.0f)
	//{
		sumDelta -= 1.0f;
		if (!disabled && GetDistanceTo(jugador) >= MaxDistance)
		{
			disabled = true;
			//Disable AI and other components
			// Hides visible components
			SetActorHiddenInGame(true);

			// Disables collision components
			SetActorEnableCollision(false);
		}
		else if (disabled && GetDistanceTo(jugador) < MaxDistance)
		{
			//TODO Move the NPC to an empty "home" and activate it
			disabled = false;
			//Enable AI and other components
			// Hides visible components
			SetActorHiddenInGame(false);

			// Disables collision components
			SetActorEnableCollision(true);
			PointReached();
		}
	//}

}

// Called to bind functionality to input
void ABasicNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasicNPC::AddWaypoint(FVector vector)
{
	ListOfObjectives.Add(vector);
}

void ABasicNPC::PointReached()
{
	if (ListOfObjectives.Num() <= 0)	return;
	if (++currentWaypoint >= ListOfObjectives.Num())
		currentWaypoint = 0;
	Destination = ListOfObjectives[currentWaypoint];
	if(!disabled)
		EmpezarNavegar();
}

void ABasicNPC::EmpezarNavegar_Implementation()
{
}

