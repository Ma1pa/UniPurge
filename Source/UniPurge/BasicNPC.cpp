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
void ABasicNPC::Iniciar(WorldGenerator* gen, int tile)
{
	generador = gen;
	position = tile;
	//SetActorLocation(generador->RetrievePosition(position));
	
}

void ABasicNPC::UpdatePatrol()
{
	
	ListOfObjectives.Empty();
	if (generador->GetWaypoints(position) != nullptr)
	{
		ListOfObjectives.Add(generador->GetWaypoints(position)[0]);
		ListOfObjectives.Add(generador->GetWaypoints(position)[1]);
		ListOfObjectives.Add(generador->GetWaypoints(position)[2]);
		ListOfObjectives.Add(generador->GetWaypoints(position)[3]);
	}
	PointReached(false);
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

	//FVector pos = { GetActorLocation().X,GetActorLocation().Y,0 };
	//FVector otro = { jugador->GetActorLocation().X,jugador->GetActorLocation().Y,0 };
	if (generador->IsFarAway(position, jugador->GetActorLocation()))
	{
		generador->StorePosition(position, FVector{ GetActorLocation().X,GetActorLocation().Y,250.0f });
		generador->GetOppositeTile(position, jugador->GetActorLocation());
		Destroy();
	}

}

// Called to bind functionality to input
void ABasicNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasicNPC::PointReached( bool isDefined)
{
	if (!isDefined)
	{
		if (ListOfObjectives.Num() <= 0)	return;
		if (++currentWaypoint >= ListOfObjectives.Num())
			currentWaypoint = 0;
		Destination = ListOfObjectives[currentWaypoint];
	}
	if(!disabled)
		EmpezarNavegar();
}

void ABasicNPC::EmpezarNavegar_Implementation()
{
}

