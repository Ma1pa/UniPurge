// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h"

// Sets default values
ATrap::ATrap()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	RootComponent = StaticMesh;

	

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

float ATrap::GetCurrentCost()
{
	return ListOfCosts[currentNumber];
}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	isShift = true;
	
}

void ATrap::LoadModel()
{
	if (ListOfMeshes.Num() > 0)
	{
		StaticMesh->SetStaticMesh(ListOfMeshes[0]);
		SetActorRelativeScale3D({ ListOfScale[0], ListOfScale[0], ListOfScale[0] });
		currentNumber = 0;
	}
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void ATrap::PlaceTrap()
{
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	TimeLeft = MaxTime;
	isShift = true;
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!isShift)
	{
		TimeLeft -= DeltaTime;
		if (TimeLeft <= 0)
		{
			Destroy();
		}
	}
}

