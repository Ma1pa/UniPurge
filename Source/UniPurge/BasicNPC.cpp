// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicNPC.h"


// Sets default values
ABasicNPC::ABasicNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	currentWaypoint = 0;
	disabled = false;
	MaxDistance = 4800.0f;

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

	FVector pos = { GetActorLocation().X,GetActorLocation().Y,0 };
	FVector otro = { jugador->GetActorLocation().X,jugador->GetActorLocation().Y,0 };
	if (FVector::Distance(pos,otro) > MaxDistance)
	{
		//The operation wanted is otro-pos; which turns into otro - pos
		//To move them a bit inwards to avoid permastuck, we reduce the radius a bit so we add |pos|/pos
		FVector destino = { ((otro.X * 2) - pos.X) + (abs(pos.X - otro.X)/(pos.X - otro.X)) * 200, (otro.Y * 2) - pos.Y + (abs(pos.Y - otro.Y) / (pos.Y - otro.Y)) * 200, 250.0};
		SetActorLocation(destino);
		//Update the objectives according to the block assigned (closest tile)
		ListOfObjectives.Empty();
		AddWaypoint({ (destino.X + GridToCoordMult -200), destino.Y - 200, 250.0 });
		AddWaypoint({ destino.X - 200, (destino.Y + GridToCoordMult - 200), 250.0 });
		AddWaypoint({ (destino.X + GridToCoordMult - 200), (destino.Y + GridToCoordMult - 200), 250.0 });
		PointReached();
	
	}

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

